// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcedurallyGeneratedMap.h"
#include "ProceduralMeshComponent.h"
#include "../Noise/Modules/billow.h"
#include "../Noise/Modules/ridgedmulti.h"
//#include "../../../ThirdParty/LibNoise/Includes/noise.h"
#include "KismetProceduralMeshLibrary.h"


#include "EngineUtils.h"
#include "LevelGenerator.h"
#include "MapFragment.h"

// Sets default values
AProcedurallyGeneratedMap::AProcedurallyGeneratedMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh Component"));
	BillowScale = 1000.0f;
	BillowRoughness = 0.1f;
	RFMScale = 1000.0f;
	RFMRoughness = 0.1f;
	bRegenerateMap = false;
	VertexDistanceCheck = 750.0f;
	SmoothingHeightCheck = 100.0f;
}

// Called when the game starts or when spawned
void AProcedurallyGeneratedMap::BeginPlay()
{
	Super::BeginPlay();

	if(LevelGenerator && !LevelGenerator->IsGenerationComplete())
	{
		LevelGenerator->OnGenerationComplete.AddDynamic(this, &AProcedurallyGeneratedMap::GenerateMap);
	}
	else
	{
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
	ClearMap();
	
	for (TActorIterator<AMapFragment> It(GetWorld()); It; ++It)
	{
		AllLevelPositions.Add(It->GetActorLocation());
	}
	
	noise::module::Billow Billow;
	Billow.SetFrequency(16);
	noise::module::RidgedMulti RFM;
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

	TArray<FVector> MovedVerticesForLevel;

	// Initial generation of the mesh
	for (int Row = 0; Row < Height; Row++)
	{
		for (int Col = 0; Col < Width; Col++)
		{
			float X = Col * GridSize;
			float Y = Row * GridSize;
			float Z = FMath::Abs(Billow.GetValue(Col * BillowRoughness, Row * BillowRoughness, 0) * BillowScale);
			Z += FMath::Abs(RFM.GetValue(Col * RFMRoughness, Row * RFMRoughness, 0) * RFMScale);
			
			Vertices.Add(FVector(X, Y, Z));
			UVCoords.Add(FVector2D(Col, Row));
		}
	}

	for (FVector v : AllLevelPositions)
	{
		bool CanMoveVertex = true;
		for (auto It = Vertices.CreateIterator() ; It ; It++)
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
					
			if(FVector::Dist(GetVertexWorldPosition(FVector(It->X, It->Y, 0)), FVector(v.X, v.Y, 0)) < VertexDistanceCheck)
			{
				if(!MovedVerticesForLevel.Contains(*It))
				{
					//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetVertexWorldPosition(Vertex).ToString())
					It->Z = v.Z;
					MovedVerticesForLevel.Add(*It);
				}
			}
		}
	}
	
	for (int Row = 0; Row < Height; Row++)
	{
		for (int Col = 0; Col < Width; Col++)
		{
			if(MovedVerticesForLevel.Contains(Vertices[Row * Width + Col]))
			{
				continue;
			}
			
			if((Row - 1) * Width + Col > 0)
			{
				if(FVector::Dist(FVector(0,0,Vertices[Row * Width + Col].Z), FVector(0,0,Vertices[(Row - 1) * Width + Col].Z)) >= SmoothingHeightCheck)
				{
					Vertices[Row * Width + Col].Z = FMath::Lerp(Vertices[Row * Width + Col].Z, Vertices[(Row - 1) * Width + Col].Z, SmoothFactor);
				}
			}
   
			if((Row) * Width + (Col - 1) > 0)
			{
				if(FVector::Dist(FVector(0,0,Vertices[Row * Width + Col].Z), FVector(0,0,Vertices[(Row) * Width + (Col - 1)].Z)) >= SmoothingHeightCheck)
				{
					Vertices[Row * Width + Col].Z = FMath::Lerp(Vertices[Row * Width + Col].Z, Vertices[(Row) * Width + (Col - 1)].Z, SmoothFactor);
				}
			}
			
		}
	}
	
	for (int Row = Height - 1; Row > 0; Row--)
	{
		for (int Col = Width - 1; Col > 0; Col--)
		{
			if(MovedVerticesForLevel.Contains(Vertices[Row * Width + Col]))
			{
				continue;
			}
		
			if((Row + 1) * Width + Col < Vertices.Num())
			{
				if(FVector::Dist(FVector(0,0,Vertices[Row * Width + Col].Z), FVector(0,0,Vertices[(Row + 1) * Width + Col].Z)) >= SmoothingHeightCheck)
				{
					Vertices[Row * Width + Col].Z = FMath::Lerp(Vertices[Row * Width + Col].Z, Vertices[(Row + 1) * Width + Col].Z, SmoothFactor);
				}
			}
			
			if((Row) * Width + (Col + 1) < Vertices.Num())
	         {
		        if(FVector::Dist(FVector(0,0,Vertices[Row * Width + Col].Z), FVector(0,0,Vertices[(Row) * Width + (Col + 1)].Z)) >= SmoothingHeightCheck)
		        {
			        Vertices[Row * Width + Col].Z = FMath::Lerp(Vertices[Row * Width + Col].Z, Vertices[(Row) * Width + (Col + 1)].Z, SmoothFactor);
		        }
	         }
		}
	}
	
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
	
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVCoords, Normals, Tangents);
	
	MeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UVCoords, TArray<FColor>(), Tangents, true);

}

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

