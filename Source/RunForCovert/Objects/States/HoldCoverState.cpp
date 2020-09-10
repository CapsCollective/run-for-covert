// Caps Collective 2020


#include "HoldCoverState.h"

UHoldCoverState::UHoldCoverState()
{
    // Set field default values
    TimeToCover = 1.5f;
    TimeStarted = 0.f;
}

void UHoldCoverState::OnEnter(AEnemyAIController& Owner)
{
    // Initiate agent crouching and start the timer
    Owner.Agent->SetCrouching(true);
    TimeStarted = Owner.GetWorld()->GetTimeSeconds();
}

void UHoldCoverState::OnExit(AEnemyAIController& Owner)
{
    // Reset crouching
    Owner.Agent->SetCrouching(false);
}

void UHoldCoverState::OnUpdate(AEnemyAIController& Owner)
{
    // Nullify cover validity once the timer ends
    if (TimeStarted + TimeToCover <= Owner.GetWorld()->GetTimeSeconds())
    {
        Owner.TakenValidCover = false;
    }
}
