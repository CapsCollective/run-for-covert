// Caps Collective 2020


#include "MapHandle.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "../GameModes/DefaultGameModeBase.h"
#include "LevelGenerator.h"
#include "MapAttachmentPoint.h"

AMapHandle::AMapHandle()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AMapHandle::BeginPlay()
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
            AttachmentPoints.Add(AttachmentPoint);
        }
    }
}

TArray<AMapAttachmentPoint*> AMapHandle::GetAttachmentPoints()
{
    return AttachmentPoints;
}