// Caps Collective 2020


#include "Cover.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

#define OUT

ACover::ACover()
{
	PrimaryActorTick.bCanEverTick = false;
	GetOwner();
}

void ACover::BeginPlay()
{
	Super::BeginPlay();

	// Populate the cover points field
	GetComponents(OUT CoverPoints);
}

UCoverPositionComponent* ACover::FindValidCoverPoint(FVector CoverFromPosition)
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

UCoverPositionComponent* ACover::GetRandomCover()
{
    return CoverPoints[FMath::RandRange(0, CoverPoints.Num()-1)];
}