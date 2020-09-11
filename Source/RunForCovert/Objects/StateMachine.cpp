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

    // Check for any valid transitions on the current state
    UClass* Transition = CurrentState->ToTransition();
    if(Transition)
    {
        // Find the new state object
        for (auto It = States.CreateConstIterator(); It; It++)
        {
            if ((*It)->IsA(Transition))
            {
                // Run state machine transition
                CurrentState->OnExit();
                CurrentState = *It;
                CurrentState->OnEnter();
                break;
            }
        }
    }

    CurrentState->OnUpdate();
}

void UStateMachine::Initialise(AEnemyAIController* ObjectOwner)
{
    if (States.Num() == 0) { return; }
    Owner = ObjectOwner;
    // Set the first defined state in the map as the first current state
    CurrentState = States[0];

    // Initialise all owned states
    for (auto It = States.CreateConstIterator(); It; It++)
    {
        (*It)->Initialise(Owner);
    }

    Super::Initialise(Owner);
}
