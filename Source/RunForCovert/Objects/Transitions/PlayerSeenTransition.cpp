// Caps Collective 2020


#include "PlayerSeenTransition.h"

bool UPlayerSeenTransition::ToTransition(AEnemyAIController& Owner) const
{
    return Owner.Agent->bChasePlayer;
}
