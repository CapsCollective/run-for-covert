// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../State.h"
#include "FireState.generated.h"

/**
 * 
 */
UCLASS()
class RUNFORCOVERT_API UFireState : public UState
{
	GENERATED_BODY()

public:

    virtual void OnEnter(AEnemyAIController& Owner);
    virtual void OnExit(AEnemyAIController& Owner);
    virtual void OnUpdate(AEnemyAIController& Owner);

private:
	float Timer;
	
};
