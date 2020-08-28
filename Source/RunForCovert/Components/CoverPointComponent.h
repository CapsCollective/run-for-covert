// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CoverPointComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNFORCOVERT_API UCoverPointComponent : public USceneComponent // TODO rename this
{

	GENERATED_BODY()

public:	

	UCoverPointComponent();

    // Public functions

    bool DoesProvideCover(FVector &CoverFromPosition);

};
