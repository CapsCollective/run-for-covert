// Caps Collective 2020


#include "FiringRangeTransition.h"

bool UFiringRangeTransition::ToTransition(AEnemyAIController& Owner) const
{
    return Owner.Agent->GetDistanceTo(Owner.Player) < Owner.Agent->FiringRange;
}