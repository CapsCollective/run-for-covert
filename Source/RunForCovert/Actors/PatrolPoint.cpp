// Caps Collective 2020


#include "PatrolPoint.h"


bool APatrolPoint::TrySetNext(APatrolPoint* Patrol)
{
    if (!NextPatrol)
    {
        NextPatrol = Patrol;
        return true;
    }
    return false;
}
