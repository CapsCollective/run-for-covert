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
	UE_LOG(LogTemp,Warning,TEXT("GENERATING MAP %i"), LevelGenerator->GetMaxLevelRadius());
	
	for (TActorIterator<AMapFragment> It(GetWorld()); It; ++It)
	{
		LevelPositionsAtZero.Add(It->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *It->GetActorLocation().ToString());
		//It->Destroy();
	}
	
	noise::module::Billow Billow;
	Billow.SetSeed(FMath::RandRange(-10000, 10000));
	Billow.SetFrequency(16);
	noise::module::RidgedMulti RFM;
	RFM.SetSeed(FMath::RandRange(-10000, 10000));
	
	
	for (int Row = 0; Row < Height; Row++)
	{
		for (int Col = 0; Col < Width; Col++)
		{
			float X = Col * GridSize;
			float Y = Row * GridSize;
			float Z = FMath::Abs(Billow.GetValue(Col * BillowRoughness, Row * BillowRoughness, 0) * BillowScale);
			Z += FMath::Abs(RFM.GetValue(Col * RFMRoughness, Row * RFMRoughness, 0) * RFMScale);
	
			for (FVector v : LevelPositionsAtZero)
			{
				if(FVector::Dist(FVector(X, Y, 0) + GetActorLocation(), FVector(v.X, v.Y, 0)) < 600)
				{
						Z = v.Z;
				}
			}
			
			Vertices.Add(FVector(X, Y, Z));
			UVCoords.Add(FVector2D(Col, Row));
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

void AProcedurallyGeneratedMap::ClearMap()
{
	Vertices.Empty();
	Triangles.Empty();
	UVCoords.Empty();
	MeshComponent->ClearAllMeshSections();
}

