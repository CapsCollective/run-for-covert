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
    int32 MaxFragments;

    UPROPERTY(EditAnywhere)
    int32 MaxAttemptsPerFragment;

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<class AMapFragment>> MapFragments;

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    int32 Attempts = 0;

    int32 ActiveFragments = 0;

    FTimerHandle TimerHandle;

    class AMapAttachmentPoint* CurrentAttachmentPoint;

    TArray<class AMapAttachmentPoint*> OpenAttachmentPoints;

    // Private Functions

    void TrySpawnFragment();

    static AMapAttachmentPoint* TryPlaceFragment(AMapFragment* MapFragment, AMapAttachmentPoint* CurrentAttachmentPoint);

    static bool TryAttachPoint(AMapFragment* MapFragment, AMapAttachmentPoint* NewAttachmentPoint,
                        AMapAttachmentPoint* CurrentAttachmentPoint);

    AMapFragment* LoadRandomLevel();
	
};
