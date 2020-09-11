// Caps Collective 2020


#include "CombatStateMachine.h"
#include "HoldCoverState.h"
#include "FireState.h"
#include "MoveCoverState.h"
#include "PatrolState.h"

void UCombatStateMachine::Initialise()
{
    // Create state objects
    States = {
            NewObject<UMoveCoverState>(),
            NewObject<UHoldCoverState>(),
            NewObject<UFireState>(),
    };

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

UClass* UCombatStateMachine::ToTransition(AEnemyAIController& Owner) const
{
    return !Owner.Agent->bChasePlayer ? UPatrolState::StaticClass() : nullptr;
}