// Caps Collective 2020


#include "Cover.h"
#include "Engine/World.h"

ACover::ACover()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACover::BeginPlay()
{
	Super::BeginPlay();

	// Populate the cover points field
	GetComponents(OUT CoverPoints);
}

UCoverPositionComponent* ACover::FindCover(FVector CoverFromPosition)
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