// Caps Collective 2020


#include "Cover.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"


ACover::ACover()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACover::BeginPlay()
{
	Super::BeginPlay();

	// Populate the cover points field
	GetComponents(OUT CoverPoints);

	if (CoverPoints.Num() <= 0)
    {
	    UE_LOG(LogTemp, Error, TEXT("No cover points found for %s"), *GetName())
    }
}

UCoverPointComponent* ACover::FindValidCoverPoint(FVector CoverFromPosition)
{
    // Iterate over cover points and return any that provide sufficient cover
    for (auto It = CoverPoints.CreateConstIterator(); It; It++)
    {
        if ((*It)->DoesProvideCover(CoverFromPosition))
        {
            return *It;
        }
    }
    return nullptr;
}

UCoverPointComponent* ACover::GetFirstCover()
{
    // Return the first available cover point
    return CoverPoints[0];
}

bool ACover::IsOccupiedByOther(AActor* Actor)
{
    // Check if any cover points are occupied
    for (auto It = CoverPoints.CreateConstIterator(); It; It++)
    {
        if ((*It)->IsOccupiedByOther(Actor))
        {
            return true;
        }
    }
    return false;
}