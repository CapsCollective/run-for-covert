// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "Transition.h"
#include "StateMachine.generated.h"


UCLASS()
class RUNFORCOVERT_API UStateMachine : public UState
{
	GENERATED_BODY()

	// State machine typedefs

    typedef TPair<UTransition*, UState*> TransitionStatePair;
    typedef TArray<TransitionStatePair> Transitions;
    typedef TMap<UState*, Transitions> TransitionMap;
    
public:

    // Public overrides
    
    virtual void OnEnter(AEnemyAIController& Owner) override;
    virtual void OnUpdate(AEnemyAIController& Owner) override;
    virtual void Initialise();

    // Public fields

    TransitionMap StateTransitions;

private:

    // Private fields
    
    UPROPERTY()
    UState* CurrentState;
};
