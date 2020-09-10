// Caps Collective 2020


#include "FireState.h"

UFireState::UFireState()
{
    // Set field default values
    TimesToFire = 3;
    TimesFired = 0;
    FireDelay = 1.f;
    TimeStarted = 0.f;
}

void UFireState::OnEnter(AEnemyAIController& Owner)
{
    TimeStarted = Owner.GetWorld()->GetTimeSeconds();
    Owner.Agent->SetCrouching(false);
}

void UFireState::OnExit(AEnemyAIController& Owner)
{
    Owner.HasFinishedFiring = false;
    TimesFired = 0;
    Owner.Agent->SetCrouching(true);
}

void UFireState::OnUpdate(AEnemyAIController& Owner)
{
    // Fire at the player once the timer ends
    if (TimeStarted + FireDelay <= Owner.GetWorld()->GetTimeSeconds())
    {
        if (Owner.Agent->FireWeapon() && ++TimesFired >= TimesToFire)
        {
            Owner.HasFinishedFiring = true;
        }
    }
}