// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Actors/CoverSystem.h"
#include "DefaultGameModeBase.generated.h"

UCLASS()
class RUNFORCOVERT_API ADefaultGameModeBase : public AGameModeBase
{

	GENERATED_BODY()

public:

    // Public functions

    class ACoverSystem* GetCoverSystem();

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    UPROPERTY()
    ACoverSystem* CoverSystem;
	
};
