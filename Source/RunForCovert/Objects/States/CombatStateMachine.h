// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../StateMachine.h"
#include "CombatStateMachine.generated.h"


UCLASS()
class RUNFORCOVERT_API UCombatStateMachine : public UStateMachine
{
	GENERATED_BODY()

public:

    // Public overrides

    virtual void Initialise() override;

    virtual void OnEnter(AEnemyAIController& Owner);

    virtual void OnExit(AEnemyAIController& Owner);

    virtual UClass* ToTransition(AEnemyAIController& Owner) const override;

};
