// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapFragment.generated.h"

UCLASS()
class RUNFORCOVERT_API AMapFragment : public AActor
{
	GENERATED_BODY()
	
public:

	AMapFragment();

	// Public functions

    TArray<class AMapAttachmentPoint*> GetAttachmentPoints();

    bool AttachmentPointsClear(AMapAttachmentPoint* IgnoredPoint);

    bool HasNoOverlaps();

    void ResetLocationRotation();

protected:

    // Protected overrides

	virtual void BeginPlay() override;

private:

    // Private fields

    UPROPERTY()
    TArray<AMapAttachmentPoint*> AttachmentPoints;

};
