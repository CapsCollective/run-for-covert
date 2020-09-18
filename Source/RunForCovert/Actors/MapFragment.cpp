// Caps Collective 2020


#include "MapFragment.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "../GameModes/DefaultGameModeBase.h"
#include "MapAttachmentPoint.h"

AMapFragment::AMapFragment()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMapFragment::BeginPlay()
{
	Super::BeginPlay();

	// Receive a list of all child actors
    TArray<AActor*> AttachedActors;
    GetAttachedActors(AttachedActors);

	// Record all children that are attachment points
	for (auto It = AttachedActors.CreateConstIterator(); It; It++)
    {
        AMapAttachmentPoint* AttachmentPoint = Cast<AMapAttachmentPoint>(*It);
	    if (AttachmentPoint)
        {
            AttachmentPoint->SetOwner(this);
            AttachmentPoints.Add(AttachmentPoint);
        }
    }
}

TArray<AMapAttachmentPoint*> AMapFragment::GetAttachmentPoints()
{
    return AttachmentPoints;
}

bool AMapFragment::AttachmentPointsClear(AMapAttachmentPoint* IgnoredPoint)
{
    for (auto It = AttachmentPoints.CreateConstIterator(); It; It++)
    {
        if ((*It) == IgnoredPoint) { continue; }

        if (!(*It)->IsClear())
        {
            return false;
        }
    }
    return true;
}