// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralTerrain.generated.h"


UCLASS()
class RUNFORCOVERT_API AProceduralTerrain : public AActor
{

	GENERATED_BODY()
	
public:	

	AProceduralTerrain();

protected:

    // Protected overrides

	virtual void BeginPlay() override;

private:

	// Private members

    UPROPERTY(EditAnywhere)
    UMaterialInterface* Material;
	
	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* MeshComponent;

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
	TArray<FVector> SmoothedVertices;

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

	UPROPERTY(EditAnywhere)
	class ALevelGenerator* LevelGenerator;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> AllLevelPositions;

	UPROPERTY(EditAnywhere)
	float VertexDistanceCheck;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SmoothDistanceFromLevel;

	UPROPERTY(EditAnywhere)
	int32 SmoothingDistance;

	FVector GetVertexWorldPosition(FVector Vertex);

	UPROPERTY()
	TArray<FVector> MovedVerticesForLevel;

	// Private functions

    UFUNCTION(BlueprintCallable)
    void GenerateMap();

    void ClearMap();
	
};
