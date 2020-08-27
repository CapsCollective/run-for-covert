// Caps Collective 2020


#include "Cover.h"
#include "Engine/World.h"

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

UCoverPositionComponent* ACover::FindCover(FVector CoverFromPosition) // TODO name this better?
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
    return CoverPoints[0]; // TODO this is not actually random!
}