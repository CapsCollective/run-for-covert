// Caps Collective 2020


#include "PlayerSeenTransition.h"

bool UPlayerSeenTransition::ToTransition(AEnemyAIController* Owner) const
{
    if(Owner->bPlayerSeen())
        return true;
    
    return false;
}
