// Caps Collective 2020


#include "DefaultGameModeBase.h"
#include "Engine/World.h"

void ADefaultGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Set field default values
    CoverSystem = NewObject<UCoverSystem>();
    CoverSystem->Initialise(GetWorld());

    PatrolSystem = NewObject<UPatrolSystem>();
    PatrolSystem->Initialise(GetWorld());
}

UCoverSystem* ADefaultGameModeBase::GetCoverSystem()
{
    return CoverSystem;
}

UPatrolSystem* ADefaultGameModeBase::GetPatrolSystem()
{
    return PatrolSystem;
}