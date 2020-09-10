// Caps Collective 2020


#include "FireState.h"

void UFireState::OnEnter()
{
    // Uncrouch
}

void UFireState::OnExit()
{
    // Crouch
}

void UFireState::OnUpdate(AEnemyAIController* Owner)
{
    // Fire at the player
    Owner->FireAtPlayer();
}