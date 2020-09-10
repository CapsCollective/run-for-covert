// Caps Collective 2020


#include "StateMachine.h"

void UStateMachine::OnEnter()
{
}

void UStateMachine::OnExit()
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
                CurrentState = TransPair.Value;
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
