// Caps Collective 2020


#include "LostCoverTransition.h"

bool ULostCoverTransition::ToTransition(AEnemyAIController& Owner) const
{
    return !Owner.TakenValidCover;
}