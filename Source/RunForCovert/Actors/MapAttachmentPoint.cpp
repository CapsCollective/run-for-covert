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
    FHitResult Hit;
    FVector StartLocation = GetActorLocation();
    FVector EndLocation = StartLocation + GetActorForwardVector() * 500.f;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());

    return !GetWorld()->LineTraceSingleByChannel(OUT Hit, StartLocation, EndLocation,
                                                 ECC_WorldDynamic, QueryParams);
}