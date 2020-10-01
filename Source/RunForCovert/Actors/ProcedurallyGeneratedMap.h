// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcedurallyGeneratedMap.generated.h"

using namespace noise;

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
	int32 Width;

	UPROPERTY(EditAnywhere)
	int32 Height;

	UPROPERTY(EditAnywhere)
	float GridSize;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Vertices;

	UPROPERTY(VisibleAnywhere)
	TArray<int32> Triangles;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector2D> UVCoords;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Normals;

	UPROPERTY(VisibleAnywhere)
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

	TArray<FVector> LevelPositionsAtZero;

};
