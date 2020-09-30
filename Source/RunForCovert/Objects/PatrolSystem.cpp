// Caps Collective 2020

#include "PatrolSystem.h"
#include "EngineUtils.h"
#include "RunForCovert/Actors/PatrolPoint.h"
#include "DrawDebugHelpers.h"


void UPatrolSystem::Initialise(UWorld* InWorld)
{
    // Set the world reference
    World = InWorld;

    // Populate a list of all patrol point actors in the world
    for (APatrolPoint* PatrolPoint : TActorRange<APatrolPoint>(World))
    {
        // Add patrol point to list
        PatrolPoints.Add(PatrolPoint);
    }

    for (auto It = PatrolPoints.CreateConstIterator(); It; It++)
    {
        APatrolPoint* ClosestPoint = FindClosestPatrolPoint(*It, *It);
        if (ClosestPoint)
        {
            (*It)->AdjacentNodes.Add(ClosestPoint);
        }
    }

    // Display the graph connections
    DisplayDebugGraph(2.f);
}

// Finds the closest patrol point
APatrolPoint* UPatrolSystem::FindClosestPatrolPoint(AActor* Agent, APatrolPoint* IgnoredPatrolPoint)
{
    // Iterate through all patrol points in world
    float ClosestDistance = TNumericLimits<float>::Max();
    APatrolPoint* ClosestPatrolPoint = nullptr;
    for (APatrolPoint* PatrolPoint : TActorRange<APatrolPoint>(World))
    {
        float Distance = PatrolPoint->GetDistanceTo(Agent);
        if (Distance <= ClosestDistance && !(IgnoredPatrolPoint && PatrolPoint == IgnoredPatrolPoint))
        {
            ClosestDistance = Distance;
            ClosestPatrolPoint = PatrolPoint;
        }
    }
    return ClosestPatrolPoint;
}

void UPatrolSystem::DisplayDebugGraph(float DisplayTime)
{
    // Display debug lines
    for (APatrolPoint* Node : PatrolPoints)
    {
        for (APatrolPoint* ConnectedNode : Node->AdjacentNodes)
        {
            DrawDebugLine(World, Node->GetActorLocation(),
                          ConnectedNode->GetActorLocation(), FColor::Orange, false, DisplayTime);
        }
    }
}