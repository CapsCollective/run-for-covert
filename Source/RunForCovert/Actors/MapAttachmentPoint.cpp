// Caps Collective 2020


#include "MapAttachmentPoint.h"
#include "Components/ArrowComponent.h"

AMapAttachmentPoint::AMapAttachmentPoint()
{
	PrimaryActorTick.bCanEverTick = false;

    // Setup components
    Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    RootComponent = Arrow;
}