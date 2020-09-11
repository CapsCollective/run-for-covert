// Caps Collective 2020


#include "StateMachine.h"

void UStateMachine::OnEnter(AEnemyAIController& Owner)
{
    // Initialise the first run state
    CurrentState->OnEnter(Owner);
}

void UStateMachine::OnUpdate(AEnemyAIController& Owner)
{
    if(States.Num() == 0 || !CurrentState) { return; }

    UClass* Transition = CurrentState->ToTransition(Owner);
    if(Transition)
    {
        for (auto It = States.CreateConstIterator(); It; It++)
        {
            if ((*It)->IsA(Transition))
            {
                CurrentState->OnExit(Owner);
                CurrentState = *It;
                CurrentState->OnEnter(Owner);
                break;
            }
        }
    }

    CurrentState->OnUpdate(Owner);
}

void UStateMachine::Initialise()
{
    if (States.Num() == 0) { return; }

    // Set the first defined state in the map as the first current state
    CurrentState = States[0];
}
