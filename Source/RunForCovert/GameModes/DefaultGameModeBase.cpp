// Caps Collective 2020


#include "DefaultGameModeBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "RunForCovert/Objects/CoverSystem.h"
#include "RunForCovert/Objects/PatrolSystem.h"
#include "RunForCovert/Actors/LevelGenerator.h"

void ADefaultGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Set field default values
    CoverSystem = NewObject<UCoverSystem>();

    PatrolSystem = NewObject<UPatrolSystem>();
    PatrolSystem->Initialise(GetWorld());

    LevelGenerator = Cast<ALevelGenerator>(
            UGameplayStatics::GetActorOfClass(GetWorld(), ALevelGenerator::StaticClass()));

    // Initialise cover on completing level generation or immediately if none exist
    if (LevelGenerator && !LevelGenerator->IsGenerationComplete())
    {
        LevelGenerator->OnGenerationComplete.AddDynamic(this, &ADefaultGameModeBase::InitialiseCover);
    }
    else
    {
        InitialiseCover();
    }
}

UCoverSystem* ADefaultGameModeBase::GetCoverSystem()
{
    return CoverSystem;
}

UPatrolSystem* ADefaultGameModeBase::GetPatrolSystem()
{
    return PatrolSystem;
}

ALevelGenerator* ADefaultGameModeBase::GetLevelGenerator()
{
    return LevelGenerator;
}

void ADefaultGameModeBase::InitialiseCover()
{
    CoverSystem->Initialise(GetWorld());
}