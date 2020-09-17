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
    TArray<TSubclassOf<class AMapHandle>> MapHandleBlueprints;

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    TArray<class AMapAttachmentPoint*> OpenAttachmentPoints;

    // Private Functions

    AMapHandle* LoadRandomLevel();
	
};
