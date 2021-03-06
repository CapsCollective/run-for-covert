// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../StateMachine.h"
#include "EnemyStateMachine.generated.h"


UCLASS()
class RUNFORCOVERT_API UEnemyStateMachine : public UStateMachine
{
	GENERATED_BODY()

public:

    // Public overrides

    virtual void Initialise(AEnemyAIController* Owner) override;
	
};
