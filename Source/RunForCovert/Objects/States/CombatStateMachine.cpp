// Caps Collective 2020


#include "CombatStateMachine.h"
#include "HoldCoverState.h"
#include "FireState.h"
#include "MoveCoverState.h"
#include "PatrolState.h"

void UCombatStateMachine::Initialise(AEnemyAIController* ObjectOwner)
{
    Owner = ObjectOwner;
    
    // Create state objects
    States = {
            NewObject<UMoveCoverState>(),
            NewObject<UHoldCoverState>(),
            NewObject<UFireState>(),
    };

    // Initialise to first state
    Super::Initialise(Owner);
}

void UCombatStateMachine::OnEnter()
{
    // Set the agent to focus on the player
    Owner->SetFocus(Owner->Player, EAIFocusPriority::Gameplay);
}

void UCombatStateMachine::OnExit()
{
    // Remove the agent's player focus
    Owner->ClearFocus(EAIFocusPriority::Gameplay);
}

UClass* UCombatStateMachine::ToTransition() const
{
    return !Owner->Agent->bChasePlayer ? UPatrolState::StaticClass() : nullptr;
}