// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelGenerationDelegate);

UCLASS()
class RUNFORCOVERT_API ALevelGenerator : public AActor
{
	GENERATED_BODY()

public:

    ALevelGenerator();

    // Public fields

    UPROPERTY(EditAnywhere)
    bool bSlowGeneration;

    UPROPERTY(EditAnywhere)
    float SlowGenerationRate;

    UPROPERTY(EditAnywhere)
    int32 TargetFragments;

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<class AMapFragment>> OpeningFragments;

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<AMapFragment>> ClosingFragments;

    // Public functions

    bool IsGenerationComplete() const;

    float GetMaxLevelRadius() const;

    // Dynamic delegates

    FLevelGenerationDelegate OnGenerationComplete;

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    bool bCompletedGeneration;

    int32 ActiveFragments;

    float MaxLevelRadius;

    UPROPERTY()
    FTimerHandle TimerHandle;

    UPROPERTY()
    class AMapAttachmentPoint* CurrentAttachmentPoint;

    UPROPERTY()
    TArray<class AMapAttachmentPoint*> OpenAttachmentPoints;

    // Private Functions

    bool RunFragmentSpawn();

    bool TryAddFragment(TArray<TSubclassOf<AMapFragment>>* FragmentList, int32 Index);

    static AMapAttachmentPoint* TryPlaceFragment(AMapFragment* MapFragment, AMapAttachmentPoint* CurrentAttachmentPoint);

    static bool TryAttachPoint(AMapFragment* MapFragment, AMapAttachmentPoint* NewAttachmentPoint,
                        AMapAttachmentPoint* CurrentAttachmentPoint);

    UFUNCTION()
    void ResizeNavMesh();

    void CompleteGeneration();

    static TArray<int32> GetRandomisedIndices(int32 ArrayLength) ;
	
};
