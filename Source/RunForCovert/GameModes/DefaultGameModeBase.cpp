// Caps Collective 2020


#include "DefaultGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "RunForCovert/Objects/CoverSystem.h"
#include "RunForCovert/Objects/PatrolSystem.h"
#include "RunForCovert/Actors/LevelGenerator.h"
#include "Engine/World.h"

ADefaultGameModeBase::ADefaultGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    DefaultEnemyPawnClass = APawn::StaticClass();
    CoverSystem = nullptr;
    PatrolSystem = nullptr;
    LevelGenerator = nullptr;
}

void ADefaultGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Set field default values
    CoverSystem = NewObject<UCoverSystem>(this);
    PatrolSystem = NewObject<UPatrolSystem>(this);
    LevelGenerator = Cast<ALevelGenerator>(
            UGameplayStatics::GetActorOfClass(GetWorld(), ALevelGenerator::StaticClass()));

    // Initialise cover and patrol points on completing level generation or immediately if none exist
    if (LevelGenerator && !LevelGenerator->IsGenerationComplete())
    {
        LevelGenerator->OnGenerationComplete.AddDynamic(this, &ADefaultGameModeBase::InitialiseSystems);
    }
    else
    {
        InitialiseSystems();
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

void ADefaultGameModeBase::InitialiseSystems()
{
    // Initialise all UObject systems
    CoverSystem->Initialise();
    PatrolSystem->Initialise();
}