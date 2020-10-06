// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcedurallyGeneratedMap.h"
#include "ProceduralMeshComponent.h"
#include "../Noise/Modules/billow.h"
#include "../Noise/Modules/ridgedmulti.h"
//#include "../../../ThirdParty/LibNoise/Includes/noise.h"
#include "KismetProceduralMeshLibrary.h"


#include "EngineUtils.h"
#include "LevelGenerator.h"
#include "MapAttachmentPoint.h"
#include "MapFragment.h"
#include "RunForCovert/Components/TerrainPointComponent.h"

// Sets default values
AProcedurallyGeneratedMap::AProcedurallyGeneratedMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup of default values
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh Component"));
	BillowScale = 1000.0f;
	BillowRoughness = 0.1f;
	RFMScale = 1000.0f;
	RFMRoughness = 0.1f;
	bRegenerateMap = false;
	VertexDistanceCheck = 750.0f;
	SmoothingDistance = 0;
}

// Called when the game starts or when spawned
void AProcedurallyGeneratedMap::BeginPlay()
{
	Super::BeginPlay();

	// Waits for the level to be generated
	// Once it has been generated, the terrain will start to be generated
	if(LevelGenerator && !LevelGenerator->IsGenerationComplete())
	{
		LevelGenerator->OnGenerationComplete.AddDynamic(this, &AProcedurallyGeneratedMap::GenerateMap);
	}
	else
	{
		// Sometimes the level is generated too quickly causing the above delegate to not be called
		// This is used in those cases.
		GenerateMap();
	}
}

// Called every frame
void AProcedurallyGeneratedMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AProcedurallyGeneratedMap::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AProcedurallyGeneratedMap::GenerateMap()
{
	// Clears the map values
	ClearMap();

	// Gets the position of every MapFragment for use later in the generation process
	for (TActorIterator<AMapFragment> It(GetWorld()); It; ++It)
	{
		// Adds all the positions to an array of FVectors
		AllLevelPositions.Add(It->GetActorLocation());
		// Get all the ChildActorComponents
		TArray<UTerrainPointComponent*> Points;
		It->GetComponents(Points);
		for(UTerrainPointComponent* p : Points)
		{
			// If the name of the ChildActor Component starts with "TerrainPoint"
			// Add to the list of FVectors for use later
			AllLevelPositions.Add(p->GetComponentLocation());
		}
	}

	// Sort the array we just filled with position by the Z value
	// This is important later to avoid clipping in the levels
	Algo::SortBy(AllLevelPositions, &FVector::Z);

	// Create an instance of Billow noise
	noise::module::Billow Billow;
	Billow.SetFrequency(16);
	// Create an instance of Ridged Multi Fractal terrain
	noise::module::RidgedMulti RFM;

	// Set a seed for the terrain to use
	// This will be helpful when it comes to networking
	// If the seed in the inspector is 0, a random seed is chosen
	// Otherwise the set seed is used.
	if(Seed == 0)
	{
		Billow.SetSeed(FMath::RandRange(-10000, 10000));
		RFM.SetSeed(FMath::RandRange(-10000, 10000));
	}
	else
	{
		Billow.SetSeed(Seed);
		RFM.SetSeed(Seed);
	}

	// Initial generation of the mesh
	// Loop through all the desired positions of the mesh
	for (int Row = 0; Row < Height; Row++)
	{
		for (int Col = 0; Col < Width; Col++)
		{
			float X = Col * GridSize;
			float Y = Row * GridSize;
			// Set the Z position of the vertex according to the Billow noise
			float Z = FMath::Abs(Billow.GetValue(Col * BillowRoughness, Row * BillowRoughness, 0) * BillowScale);
			// Add to the Z position of the vertex according to the Ridged Multi Fractal noise
			Z += FMath::Abs(RFM.GetValue(Col * RFMRoughness, Row * RFMRoughness, 0) * RFMScale);

			// Add them all to a list
			Vertices.Add(FVector(X, Y, Z));
			UVCoords.Add(FVector2D(Col, Row));
		}
	}

	// For each of the MapFragment and TerrainPoint position we got earlier
	for (FVector v : AllLevelPositions)
	{
		// Check if we can move the vertex
		// We do a check to see if the positions of the fragment are on top of each other
		// This avoids clipping and terrain being raised through MapFragments
		bool CanMoveVertex = true;
		for (auto It = Vertices.CreateIterator() ; It ; ++It)
		{
			for (FVector v2 : AllLevelPositions)
			{
				if(FMath::IsNearlyEqual(v.X, v2.X, 10) &&
                 FMath::IsNearlyEqual(v2.X, v2.Y, 10) &&
                 v.Z > v2.Z)
				{
					CanMoveVertex = false;
				}
			}
	
			if(!CanMoveVertex) { continue; }

			// If they can be moved, we set the vertices in a radius around the FVector to the height of the MapFragment
			if(FVector::Dist(GetVertexWorldPosition(FVector(It->X, It->Y, 0)), FVector(v.X, v.Y, 0)) <= VertexDistanceCheck)
			{
				if(!MovedVerticesForLevel.Contains(*It))
				{
					It->Z = v.Z;
					// We keep a list of the moved vertices to use for later when we smooth the terrain
					MovedVerticesForLevel.Add(*It);
				}
			}
		}
	}

	// Create a copy of the vertex array so we can do a smoothing pass
	// Without being destructive of our original array causing issues
	SmoothedVertices = Vertices;
	
	for (int Row = 0; Row < Height; Row++)
	{
		for (int Col = 0; Col < Width; Col++)
		{		

			// We do a check to see if the vertex was one set by the MapFragment
			// If it is, we then check the distance of it from the center
			// If it is too close we ignore it and skip the smoothing process
			bool bSkip = false;
			for(FVector v : AllLevelPositions)
			{
				if(FVector::Dist(GetVertexWorldPosition( Vertices[Row * Width + Col]), v) < VertexDistanceCheck * SmoothDistanceFromLevel)
				{
					bSkip = true;
				}
			}

			if(bSkip) {continue;}

			// Start getting the average of the vertices around the current one
			float Average = Vertices[Row * Width + Col].Z;

			// The smoothing process requires us to get the current vertex, and every vertex around it
			// And calculating the average
			// I also used a for loop to extend how far out the average can search
			// This creates a much smoother terrain

			// So we get the current vertex and then loop in a direction getting each vertex Z position
			// This loop will keep going in the positive Y direction and add to our average float each Z position
			for(int i = 0; i < SmoothingDistance; i++)
			{
				if((Row - i) * Width + Col > 0)
				{
					Average += Vertices[(Row - i) * Width + Col].Z;
				}
			}

			// This loop will keep going in the positive X direction and add to our average float each Z position
			for(int i = 0; i < SmoothingDistance; i++)
			{
				if((Row) * Width + (Col - i) > 0)
				{
					Average += Vertices[(Row) * Width + (Col - i)].Z;
				}
			}

			// This loop will keep going in the negative Y direction and add to our average float each Z position
			for(int i = 0; i < SmoothingDistance; i++)
			{
				if((Row + i) * Width + Col < Vertices.Num())
				{
					Average += Vertices[(Row + i) * Width + Col].Z;
				}
			}

			// This loop will keep going in the negative X direction and add to our average float each Z position
			for(int i = 0; i < SmoothingDistance; i++)
			{
				if((Row) * Width + (Col + i) < Vertices.Num())
				{
					Average += Vertices[(Row) * Width + (Col + i)].Z;
				}
			}

			// Set the height of the current vertex to the average of all the vertices we parsed.
			SmoothedVertices[Row * Width + Col].Z = Average / (5 * SmoothingDistance);
			
		}
	}

	// Create the triangles for the mesh
	// This was covered in the lab.
	for (int Row = 0; Row < Height - 1; Row++)
	{
		for (int Col = 0; Col < Width - 1; Col++)
		{
			Triangles.Add(Row * Width + Col);
			Triangles.Add((Row + 1) * Width + Col);
			Triangles.Add((Row * Width) + Col + 1);
			Triangles.Add((Row * Width) + Col + 1);
			Triangles.Add((Row + 1) * Width + Col);
			Triangles.Add((Row + 1) * Width + Col + 1);
		}
	}

	// Calculate tangents and normals for our mesh using the data passed in.
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(SmoothedVertices, Triangles, UVCoords, Normals, Tangents);

	// Create the mesh
	// Also covered in the lab
	MeshComponent->CreateMeshSection(0, SmoothedVertices, Triangles, Normals, UVCoords, TArray<FColor>(), Tangents, true);

}

// Calculate the world position of a vertex position.
FVector AProcedurallyGeneratedMap::GetVertexWorldPosition(FVector Vertex)
{
	return (FVector(Vertex.X, Vertex.Y, Vertex.Z)) + GetActorLocation();
}

void AProcedurallyGeneratedMap::ClearMap()
{
	Vertices.Empty();
	Triangles.Empty();
	UVCoords.Empty();
	MeshComponent->ClearAllMeshSections();
}

