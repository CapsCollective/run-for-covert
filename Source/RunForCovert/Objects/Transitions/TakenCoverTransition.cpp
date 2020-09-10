// Caps Collective 2020


#include "TakenCoverTransition.h"

bool UTakenCoverTransition::ToTransition(AEnemyAIController& Owner) const
{
    return Owner.CoverPoint != nullptr;
}