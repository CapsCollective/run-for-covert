// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "Transition.h"

#include "StateMachine.generated.h"

/**
 * 
 */
UCLASS()
class RUNFORCOVERT_API UStateMachine : public UState
{
	GENERATED_BODY()
    
    typedef TPair<UTransition*, UState*> TransitionStatePair;
    typedef TArray<TransitionStatePair> Transitions;
    typedef TMap<UState*, Transitions> TransitionMap;
    
public:
    
    virtual void OnEnter(AEnemyAIController* Owner) override;
    virtual void OnExit(AEnemyAIController* Owner) override;
    virtual void OnUpdate(AEnemyAIController* Owner) override;

    void Initialise();
    TransitionMap StateTransitions;
    
    UPROPERTY()
    UState* CurrentState;
};
