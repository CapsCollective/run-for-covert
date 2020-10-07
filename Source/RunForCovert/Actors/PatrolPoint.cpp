// Caps Collective 2020


#include "PatrolPoint.h"


bool APatrolPoint::TrySetNext(APatrolPoint* Patrol)
{
    // Set the value if it is unset
    if (!NextPatrol)
    {
        NextPatrol = Patrol;
        return true;
    }
    return false;
}
