// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"


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
    int32 MaxFragments;

    UPROPERTY(EditAnywhere)
    int32 MaxAttemptsPerFragment;

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<class AMapFragment>> MapFragments;

    // Public functions

    bool IsGenerationComplete() const;

    // Dynamic delegates

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelGenerationDelegate);
    FLevelGenerationDelegate OnGenerationComplete;

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    bool bGenerationComplete;

    int32 Attempts = 0;

    int32 ActiveFragments = 0;

    FTimerHandle TimerHandle;

    class AMapAttachmentPoint* CurrentAttachmentPoint;

    TArray<class AMapAttachmentPoint*> OpenAttachmentPoints;

    // Private Functions

    bool TrySpawnFragment();

    static AMapAttachmentPoint* TryPlaceFragment(AMapFragment* MapFragment, AMapAttachmentPoint* CurrentAttachmentPoint);

    static bool TryAttachPoint(AMapFragment* MapFragment, AMapAttachmentPoint* NewAttachmentPoint,
                        AMapAttachmentPoint* CurrentAttachmentPoint);

    static TArray<int32> GetRandomisedIndices(int32 ArrayLength) ;
	
};
