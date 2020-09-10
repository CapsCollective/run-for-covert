// Caps Collective 2020


#include "StateMachine.h"

void UStateMachine::OnEnter(AEnemyAIController* Owner)
{
}

void UStateMachine::OnExit(AEnemyAIController* Owner)
{
}

void UStateMachine::OnUpdate(AEnemyAIController* Owner)
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
                CurrentState->OnExit(Owner);
                CurrentState = TransPair.Value;
                CurrentState->OnEnter(Owner);
                break;
            }
        }
    }
    
    CurrentState->OnUpdate(Owner);
}

void UStateMachine::Initialise()
{
    TArray<UState*> Keys;
    StateTransitions.GetKeys(OUT Keys);
    CurrentState = Keys[0];
}
