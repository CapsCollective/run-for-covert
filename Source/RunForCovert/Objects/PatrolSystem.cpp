// Caps Collective 2020

#include "PatrolSystem.h"
#include "EngineUtils.h"
#include "RunForCovert/Actors/PatrolPoint.h"
#include "DrawDebugHelpers.h"
#include "../Characters/EnemyCharacterBase.h"
#include "../GameModes/LevelGameMode.h"


void UPatrolSystem::Initialise()
{
    // Populate a list of all patrol point actors in the world
    for (APatrolPoint* PatrolPoint : TActorRange<APatrolPoint>(GetWorld()))
    {
        // Add patrol point to list
        PatrolPoints.Add(PatrolPoint);
    }

    // Get reference to the game mode
    ALevelGameMode* GameMode = Cast<ALevelGameMode>(GetWorld()->GetAuthGameMode());
    if (!GameMode) { return; }


    // Generate the patrol connections and spawn enemies
    for (auto It = PatrolPoints.CreateConstIterator(); It; It++)
    {
        APatrolPoint* ClosestPoint = FindClosestPatrolPoint(*It, *It);
        if (ClosestPoint)
        {
            // Add the connection to the point's adjacent nodes
            (*It)->TrySetNext(ClosestPoint);

            // Spawn an enemy at the point if it is flagged as such
            if ((*It)->bSpawnsEnemy)
            {
                GetWorld()->SpawnActor<AEnemyCharacterBase>(
                        GameMode->DefaultEnemyPawnClass, (*It)->GetActorLocation(), (*It)->GetActorRotation());
            }
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
    for (APatrolPoint* PatrolPoint : TActorRange<APatrolPoint>(GetWorld()))
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
    for (APatrolPoint* PatrolPoint : PatrolPoints)
    {
        DrawDebugLine(GetWorld(), PatrolPoint->GetActorLocation(),
                      PatrolPoint->NextPatrol->GetActorLocation(), FColor::Orange, false, DisplayTime);
    }
}