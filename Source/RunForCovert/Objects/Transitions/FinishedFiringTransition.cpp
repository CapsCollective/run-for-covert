// Caps Collective 2020


#include "FinishedFiringTransition.h"

bool UFinishedFiringTransition::ToTransition(AEnemyAIController& Owner) const
{
    return Owner.HasFinishedFiring;
}