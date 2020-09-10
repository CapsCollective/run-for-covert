// Caps Collective 2020


#include "FireState.h"

void UFireState::OnEnter(AEnemyAIController& Owner)
{
    TimeStarted = Owner.GetWorld()->GetTimeSeconds();
    Owner.Agent->SetCrouching(false);
}

void UFireState::OnExit(AEnemyAIController& Owner)
{
    Owner.HasFired = false;
}

void UFireState::OnUpdate(AEnemyAIController& Owner)
{
    // Fire at the player once the timer ends
    if (TimeStarted + TimeToFire <= Owner.GetWorld()->GetTimeSeconds())
    {
        Owner.FireAtPlayer();
        Owner.Agent->SetCrouching(true);
        Owner.HasFired = true;
    }
}