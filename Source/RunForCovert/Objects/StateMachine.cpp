// Caps Collective 2020


#include "StateMachine.h"

void UStateMachine::OnEnter()
{
    // Initialise the first run state
    CurrentState->OnEnter();
}

void UStateMachine::OnUpdate()
{
    if(States.Num() == 0 || !CurrentState) { return; }

    UClass* Transition = CurrentState->ToTransition();
    if(Transition)
    {
        for (auto It = States.CreateConstIterator(); It; It++)
        {
            if ((*It)->IsA(Transition))
            {
                CurrentState->OnExit();
                CurrentState = *It;
                CurrentState->OnEnter();
                break;
            }
        }
    }

    CurrentState->OnUpdate();
}

void UStateMachine::Initialise(AEnemyAIController* Owner)
{
    if (States.Num() == 0) { return; }

    // Set the first defined state in the map as the first current state
    CurrentState = States[0];

    // Initialise all owned states
    for (auto It = States.CreateConstIterator(); It; It++)
    {
        (*It)->Initialise(Owner);
    }

    Super::Initialise(Owner);
}
