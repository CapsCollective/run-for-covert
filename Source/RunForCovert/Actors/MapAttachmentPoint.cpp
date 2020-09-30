// Caps Collective 2020


#include "MapAttachmentPoint.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

AMapAttachmentPoint::AMapAttachmentPoint()
{
	PrimaryActorTick.bCanEverTick = false;

    // Setup components
    Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    RootComponent = Arrow;
}

bool AMapAttachmentPoint::IsClear()
{
    // Calculate the trace values
    FHitResult Hit;
    FVector StartLocation = GetActorLocation();
    FVector EndLocation = StartLocation + GetActorForwardVector() * 500.f;

    // Add the owner to the ignore list
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());

    // Return whether the trace was clear
    return !GetWorld()->LineTraceSingleByChannel(OUT Hit, StartLocation, EndLocation,
                                                 ECC_WorldDynamic, QueryParams);
}