// Caps Collective 2020


#include "PlayerUnseenTransition.h"

bool UPlayerUnseenTransition::ToTransition(AEnemyAIController* Owner) const
{
    return !Owner->bPlayerSeen();
}
