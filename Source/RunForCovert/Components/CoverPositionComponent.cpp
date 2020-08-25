// Caps Collective 2020


#include "CoverPositionComponent.h"
#include "Engine/World.h"

#define OUT

UCoverPositionComponent::UCoverPositionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UCoverPositionComponent::DoesProvideCover(FVector &CoverFromPosition)
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