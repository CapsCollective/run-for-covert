// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../StateMachine.h"
#include "CombatStateMachine.generated.h"

/**
 * 
 */
UCLASS()
class RUNFORCOVERT_API UCombatStateMachine : public UStateMachine
{
	GENERATED_BODY()

public:

    virtual void Initialise() override;
    virtual void OnEnter(AEnemyAIController& Owner) override;
};
