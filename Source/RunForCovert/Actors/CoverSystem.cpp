// Caps Collective 2020


#include "CoverSystem.h"
#include "EngineUtils.h"
#include "../Actors/Cover.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#define OUT

ACoverSystem::ACoverSystem()
{
	PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    CoverRadius = 1000.f;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), OUT IgnoredActors);
}

void ACoverSystem::BeginPlay()
{
	Super::BeginPlay();

	// Get a list of all cover actors in the world
    for (ACover* Cover : TActorRange<ACover>(GetWorld()))
    {
        CoverActors.Add(Cover);
    }

    if (CoverActors.Num() < 3) {
        UE_LOG(LogTemp, Error, TEXT("Cannot generate cover system for less than three actors"))
        return;
    }

    // Generate a random geometric graph of R depth between cover items
    TArray<ACover*> SortedCoverActors = CoverActors;
    for (auto CurrentCoverIt = CoverActors.CreateConstIterator(); CurrentCoverIt; CurrentCoverIt++)
    {
        for (auto SortedCoverIt = SortedCoverActors.CreateConstIterator(); SortedCoverIt; SortedCoverIt++)
        {
            if ((*SortedCoverIt)->GetDistanceTo(*CurrentCoverIt) <= CoverRadius && *SortedCoverIt != *CurrentCoverIt)
            {
                (*CurrentCoverIt)->AddAdjacentCover(*SortedCoverIt);
            }
        }
    }
}

UCoverPositionComponent* ACoverSystem::GetClosestValidCoverPoint(AActor* Agent, AActor* Enemy)
{
    // Iterate through all cover points in world
    float ClosestCoverDistance = TNumericLimits<float>::Max();
    UCoverPositionComponent* ClosestCoverPoint = nullptr;
    for (auto It = CoverActors.CreateConstIterator(); It; It++)
    {
        // Check if the current point is closer than
        float CoverDistance = (*It)->GetDistanceTo(Agent);
        if (CoverDistance >= ClosestCoverDistance) { continue; }

        // Check if it actually provides cover from the enemy
        UCoverPositionComponent* PotentialCover = (*It)->FindCover(Enemy->GetActorLocation(), IgnoredActors);
        if (PotentialCover)
        {
            ClosestCoverDistance = CoverDistance;
            ClosestCoverPoint = PotentialCover;
        }
    }
    return ClosestCoverPoint;
}

TArray<UCoverPositionComponent*> ACoverSystem::GetCoverPath(UCoverPositionComponent* CurrentCoverPosition, AActor* Enemy)
{
    // TODO implement path generation of valid cover points toward the enemy
    return TArray<UCoverPositionComponent*>();
}