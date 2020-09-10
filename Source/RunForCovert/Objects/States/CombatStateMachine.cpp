// Caps Collective 2020


#include "CombatStateMachine.h"
#include "HoldCoverState.h"
#include "FireState.h"
#include "MoveCoverState.h"
#include "../Transitions/FinishedMovingTransition.h"
#include "../Transitions/FinishedFiringTransition.h"
#include "../Transitions/TakenCoverTransition.h"
#include "../Transitions/LostCoverTransition.h"

void UCombatStateMachine::Initialise()
{
    // Create state objects
    UMoveCoverState* MoveCoverState = NewObject<UMoveCoverState>();
    UHoldCoverState* HoldCoverState = NewObject<UHoldCoverState>();
    //UFireState* FireState = NewObject<UFireState>();

    // Add state transitions to state machine
    StateTransitions.Add(MoveCoverState, {
            TPair<UTransition*, UState*>(NewObject<UTakenCoverTransition>(), HoldCoverState),
            //TPair<UTransition*, UState*>(NewObject<UTakenCoverTransition>(), FireState),
    });
    StateTransitions.Add(HoldCoverState, {
            TPair<UTransition*, UState*>(NewObject<ULostCoverTransition>(), MoveCoverState),
    });
//    StateTransitions.Add(FireState, {
//            TPair<UTransition*, UState*>(NewObject<UFinishedFiringTransition>(), HoldCoverState),
//    });
    
    // Initialise to first state
    Super::Initialise();
}

void UCombatStateMachine::OnEnter(AEnemyAIController& Owner)
{
    CurrentState->OnEnter(Owner);
}
