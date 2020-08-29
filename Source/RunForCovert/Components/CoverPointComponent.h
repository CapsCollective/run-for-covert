// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CoverPointComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNFORCOVERT_API UCoverPointComponent : public USceneComponent
{

	GENERATED_BODY()

public:	

	UCoverPointComponent(); // TODO cover points should be lockable for multiple AI to coordinate

    // Public functions

    bool DoesProvideCover(FVector &CoverFromPosition);

    bool TrySetOccupation(class AActor* Actor);

    void ReleaseOccupation(AActor* Actor);

    bool IsOccupiedByOther(AActor* Actor);

private:

    // Private fields

    UPROPERTY()
    AActor* Occupier;

};
