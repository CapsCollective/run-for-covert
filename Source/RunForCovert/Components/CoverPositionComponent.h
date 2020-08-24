// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CoverPositionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNFORCOVERT_API UCoverPositionComponent : public USceneComponent
{

	GENERATED_BODY()

public:	

	UCoverPositionComponent();

    // Public functions

    bool DoesProvideCover(FVector &CoverFromPosition, struct FCollisionQueryParams &QueryParams);

};
