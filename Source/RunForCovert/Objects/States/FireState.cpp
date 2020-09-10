// Caps Collective 2020


#include "FireState.h"

void UFireState::OnEnter(AEnemyAIController& Owner)
{
    Owner.Agent->SetCrouching(false);
}

void UFireState::OnExit(AEnemyAIController& Owner)
{
    Owner.HasFired = false;
}

void UFireState::OnUpdate(AEnemyAIController& Owner)
{
    // Fire at the player
    Owner.FireAtPlayer();
    Owner.HasFired = true;
}