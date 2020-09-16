// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelGenerator.generated.h"


UCLASS()
class RUNFORCOVERT_API ULevelGenerator : public UObject
{
	GENERATED_BODY()

public:

    ULevelGenerator();

    // Public functions

    void Initialise(UWorld* InWorld);

    void RegisterAttachmentPoint(class AMapAttachmentPoint* AttachmentPoint);

private:

    // Private fields

    UPROPERTY()
    UWorld* World;

    UPROPERTY()
    TArray<AMapAttachmentPoint*> AttachmentPoints;
	
};
