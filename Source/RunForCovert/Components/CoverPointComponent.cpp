// Caps Collective 2020


#include "CoverPointComponent.h"
#include "Engine/World.h"

#define OUT

UCoverPointComponent::UCoverPointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UCoverPointComponent::DoesProvideCover(FVector &CoverFromPosition)
{
    // Perform ray-trace between cover point and point to take cover from
    FHitResult Hit;
    FCollisionQueryParams QueryParams;

    return GetOwner()->ActorLineTraceSingle(
            OUT Hit,
            GetComponentLocation(),
            CoverFromPosition,
            ECC_WorldDynamic,
            QueryParams);
}