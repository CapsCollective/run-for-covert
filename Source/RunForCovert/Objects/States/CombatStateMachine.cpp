// Caps Collective 2020


#include "CombatStateMachine.h"
#include "FindCoverState.h"
#include "FireState.h"
#include "MoveCoverState.h"
#include "../Transitions/FinishedMovingTransition.h"
#include "../Transitions/FinishedFiringTransition.h"
#include "../Transitions/PlayerNearTransition.h"

void UCombatStateMachine::Initialise()
{
    // Create state objects
    UFindCoverState* FindCoverState = NewObject<UFindCoverState>();
    UMoveCoverState* MoveCoverState = NewObject<UMoveCoverState>();
    UFireState* FireState = NewObject<UFireState>();

    // Add state transitions to state machine
    StateTransitions.Add(FindCoverState, {
            TPair<UTransition*, UState*>(NewObject<UPlayerNearTransition>(), FireState),
            TPair<UTransition*, UState*>(NewObject<UPlayerNearTransition>(), MoveCoverState),
    });
    StateTransitions.Add(FireState, {
            TPair<UTransition*, UState*>(NewObject<UFinishedFiringTransition>(), FindCoverState),
    });
    StateTransitions.Add(MoveCoverState, {
            TPair<UTransition*, UState*>(NewObject<UFinishedMovingTransition>(), FindCoverState),
    });

    // Initialise to first state
    Super::Initialise();
}