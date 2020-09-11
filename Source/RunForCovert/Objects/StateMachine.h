// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "StateMachine.generated.h"


UCLASS()
class RUNFORCOVERT_API UStateMachine : public UState
{
	GENERATED_BODY()
    
public:

    // Public overrides

    virtual void Initialise() override;
    
    virtual void OnEnter(AEnemyAIController& Owner) override;

    virtual void OnUpdate(AEnemyAIController& Owner) override;

    // Public fields

    UPROPERTY()
    TArray<UState*> States;

private:

    // Private fields
    
    UPROPERTY()
    UState* CurrentState;
};
