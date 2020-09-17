// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapHandle.generated.h"

UCLASS()
class RUNFORCOVERT_API AMapHandle : public AActor
{
	GENERATED_BODY()
	
public:

	AMapHandle();

	// Public functions

    TArray<class AMapAttachmentPoint*> GetAttachmentPoints();

protected:

    // Protected overrides

	virtual void BeginPlay() override;

private:

    // Private fields

    UPROPERTY()
    TArray<AMapAttachmentPoint*> AttachmentPoints;

};
