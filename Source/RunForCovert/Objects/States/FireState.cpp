// Caps Collective 2020


#include "FireState.h"

void UFireState::OnEnter(AEnemyAIController& Owner)
{
    UE_LOG(LogTemp, Warning, TEXT("Entering Fire"))
    Owner.Agent->SetCrouching(true);
    Timer = 1.5f;
}

void UFireState::OnExit(AEnemyAIController& Owner)
{
    Owner.HasFired = false;
}

void UFireState::OnUpdate(AEnemyAIController& Owner)
{
    Timer -= Owner.GetWorld()->GetDeltaSeconds();
    if (Timer > 0.0f && Timer < 0.25f)
        Owner.Agent->SetCrouching(false);
    else if(Timer <= 0.0f)
    {
        // Fire at the player
        Owner.FireAtPlayer();
        Owner.HasFired = true;
    }
}