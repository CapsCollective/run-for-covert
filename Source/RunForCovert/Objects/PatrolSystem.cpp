// Caps Collective 2020

#include "PatrolSystem.h"
#include "EngineUtils.h"
#include "RunForCovert/Actors/Patrol.h"
#include "DrawDebugHelpers.h"


void UPatrolSystem::Initialise(UWorld* InWorld)
{
    // Set the world reference
    World = InWorld;

    // Populate a list of nodes representing all patrol actors in the
    // world wrapped in objects specifying graph attributes
    for (APatrol* Patrol : TActorRange<APatrol>(World))
    {
        // Add patrol node list
        PatrolPoints.Add(Patrol);
    }

    // Display the graph connections
    DisplayDebugGraph(2.f);
}

// Finds the closest patrol point
APatrol* UPatrolSystem::FindClosestPatrolPoint(AActor* Agent)
{
    // Iterate through all patrol points in world
    float ClosestPatrolDistance = TNumericLimits<float>::Max();
    APatrol* ClosestPatrolPoint = nullptr;
    for (APatrol* Patrol : TActorRange<APatrol>(World))
    {
        float PatrolDistance = Patrol->GetDistanceTo(Agent);
        if (PatrolDistance <= ClosestPatrolDistance)
        {
            ClosestPatrolDistance = PatrolDistance;
            ClosestPatrolPoint = Patrol;
        }
    }
    
    return ClosestPatrolPoint;
}

void UPatrolSystem::DisplayDebugGraph(float DisplayTime)
{
    // Display debug lines
    for (APatrol* Node : PatrolPoints)
    {
        for (APatrol* ConnectedNode : Node->AdjacentNodes)
        {
            DrawDebugLine(World, Node->GetActorLocation(),
                          ConnectedNode->GetActorLocation(), FColor::Orange, false, DisplayTime);
        }
    }
}