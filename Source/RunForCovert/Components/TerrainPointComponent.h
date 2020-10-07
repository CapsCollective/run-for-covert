// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TerrainPointComponent.generated.h"

/**
 * This component isn't used for anything other than to be a marker for
 * the terrain generator
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNFORCOVERT_API UTerrainPointComponent : public USceneComponent
{

	GENERATED_BODY()

};
