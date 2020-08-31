// Caps Collective 2020


#include "CoverPointComponent.h"
#include "Engine/World.h"

UCoverPointComponent::UCoverPointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    // Set field default values
    Occupier = nullptr;
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

bool UCoverPointComponent::TrySetOccupation(class AActor* Actor)
{
    if (!Occupier)
    {
        Occupier = Actor;
        return true;
    }
    return false;
}

void UCoverPointComponent::ReleaseOccupation(AActor* Actor)
{
    if (Actor != Occupier) { return; }
    Occupier = nullptr;
}

bool UCoverPointComponent::IsOccupiedByOther(AActor* Actor)
{
    return Occupier && Occupier != Actor;
}