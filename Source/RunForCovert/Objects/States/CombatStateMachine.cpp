// Caps Collective 2020


#include "CombatStateMachine.h"
#include "HoldCoverState.h"
#include "FireState.h"
#include "MoveCoverState.h"
#include "../Transitions/FiringRangeTransition.h"
#include "../Transitions/FinishedFiringTransition.h"
#include "../Transitions/TakenCoverTransition.h"
#include "../Transitions/LostCoverTransition.h"

void UCombatStateMachine::Initialise()
{
    // Create state objects
    UMoveCoverState* MoveCoverState = NewObject<UMoveCoverState>();
    UHoldCoverState* HoldCoverState = NewObject<UHoldCoverState>();
    UFireState* FireState = NewObject<UFireState>();

    // Add state transitions to state machine
    StateTransitions.Add(MoveCoverState, {
            TPair<UTransition*, UState*>(NewObject<UTakenCoverTransition>(), HoldCoverState),
    });
    StateTransitions.Add(HoldCoverState, {
            TPair<UTransition*, UState*>(NewObject<UFiringRangeTransition>(), FireState),
            TPair<UTransition*, UState*>(NewObject<ULostCoverTransition>(), MoveCoverState),
    });
    StateTransitions.Add(FireState, {
            TPair<UTransition*, UState*>(NewObject<UFinishedFiringTransition>(), HoldCoverState),
    });
    
    // Initialise to first state
    Super::Initialise();
}

void UCombatStateMachine::OnEnter(AEnemyAIController& Owner)
{
    // Set the agent to focus on the player
    Owner.SetFocus(Owner.Player, EAIFocusPriority::Gameplay);
}

void UCombatStateMachine::OnExit(AEnemyAIController& Owner)
{
    // Remove the agent's player focus
    Owner.ClearFocus(EAIFocusPriority::Gameplay);
}