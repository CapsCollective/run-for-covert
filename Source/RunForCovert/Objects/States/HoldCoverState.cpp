// Caps Collective 2020


#include "HoldCoverState.h"


void UHoldCoverState::OnEnter(AEnemyAIController& Owner)
{
    Owner.Agent->SetCrouching(true);
    TimeStarted = Owner.GetWorld()->GetTimeSeconds();
}

void UHoldCoverState::OnExit(AEnemyAIController& Owner)
{
    Owner.Agent->SetCrouching(false);
}

void UHoldCoverState::OnUpdate(AEnemyAIController& Owner)
{
    if (TimeStarted + TimeToCover <= Owner.GetWorld()->GetTimeSeconds())
    {
        Owner.TakenValidCover = false;
    }
}
