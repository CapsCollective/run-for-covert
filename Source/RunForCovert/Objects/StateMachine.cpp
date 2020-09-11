// Caps Collective 2020


#include "StateMachine.h"

void UStateMachine::OnEnter(AEnemyAIController& Owner)
{
    // Initialise the first run state
    CurrentState->OnEnter(Owner);
}

void UStateMachine::OnUpdate(AEnemyAIController& Owner)
{
    if(!CurrentState){ return; }
    if(StateTransitions.Num() == 0) { return; }
    
    auto It = StateTransitions.Find(CurrentState);
    if(It)
    {
        for(TransitionStatePair TransPair : *It)
        {
            if(TransPair.Key->ToTransition(Owner))
            {
                if(CurrentState)
                    CurrentState->OnExit(Owner);
                CurrentState = TransPair.Value;
                if(CurrentState)
                    CurrentState->OnEnter(Owner);
                break;
            }
        }
    }
    
    CurrentState->OnUpdate(Owner);
}

void UStateMachine::Initialise()
{
    // Set the first defined state in the map as the first current state
    TArray<UState*> Keys;
    StateTransitions.GetKeys(OUT Keys);
    CurrentState = Keys[0];
}
