// Caps Collective 2020


#include "DefaultGameModeBase.h"
#include "Engine/World.h"

void ADefaultGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Set field default values
    CoverSystem = GetWorld()->SpawnActor<ACoverSystem>();
}

ACoverSystem* ADefaultGameModeBase::GetCoverSystem()
{
    return CoverSystem;
}