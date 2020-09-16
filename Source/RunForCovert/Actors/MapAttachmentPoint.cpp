// Caps Collective 2020


#include "MapAttachmentPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "../GameModes/DefaultGameModeBase.h"
#include "../Objects/LevelGenerator.h"

AMapAttachmentPoint::AMapAttachmentPoint()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AMapAttachmentPoint::BeginPlay()
{
	Super::BeginPlay();

    ADefaultGameModeBase* GameMode = Cast<ADefaultGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

    if (!GameMode) { return; }

    if (!GameMode->GetLevelGenerator()) { return; }

    GameMode->GetLevelGenerator()->RegisterAttachmentPoint(this);
}

