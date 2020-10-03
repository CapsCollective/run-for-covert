// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelGenerator.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ProcedurallyGeneratedMap.generated.h"

UCLASS()
class RUNFORCOVERT_API AProcedurallyGeneratedMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcedurallyGeneratedMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	int32 Seed;
	
	UPROPERTY(EditAnywhere)
	int32 Width;

	UPROPERTY(EditAnywhere)
	int32 Height;

	UPROPERTY(EditAnywhere)
	float GridSize;

	UPROPERTY()
	TArray<FVector> Vertices;

	UPROPERTY()
	TArray<int32> Triangles;

	UPROPERTY()
	TArray<FVector2D> UVCoords;

	UPROPERTY()
	TArray<FVector> Normals;

	UPROPERTY()
	TArray<FProcMeshTangent> Tangents;

	UPROPERTY(EditAnywhere)
	float BillowScale;

	UPROPERTY(EditAnywhere)
	float BillowRoughness;

	UPROPERTY(EditAnywhere)
	float RFMScale;

	UPROPERTY(EditAnywhere)
	float RFMRoughness;

	UFUNCTION(BlueprintCallable)
	void GenerateMap();

	void ClearMap();

	UPROPERTY(EditAnywhere)
	bool bRegenerateMap;

	UPROPERTY(EditAnywhere)
	ALevelGenerator* LevelGenerator;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> AllLevelPositions;

	UPROPERTY(EditAnywhere)
	float VertexDistanceCheck;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SmoothFactor;

	UPROPERTY(EditAnywhere)
	float SmoothingHeightCheck;

	FVector GetVertexWorldPosition(FVector Vertex);

	UPROPERTY()
	TArray<FVector> MovedVerticesForLevel;
	
};
