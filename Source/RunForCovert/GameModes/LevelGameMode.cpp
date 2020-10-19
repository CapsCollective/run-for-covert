// Caps Collective 2020


#include "LevelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "RunForCovert/Objects/CoverSystem.h"
#include "RunForCovert/Objects/PatrolSystem.h"
#include "RunForCovert/Actors/LevelGenerator.h"
#include "Engine/World.h"

ALevelGameMode::ALevelGameMode()
{
    PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    DefaultEnemyPawnClass = APawn::StaticClass();
    CoverSystem = nullptr;
    PatrolSystem = nullptr;
    LevelGenerator = nullptr;
}

void ALevelGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Set the input mode to game only and hide the cursor
    GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

    // Set field default values
    CoverSystem = NewObject<UCoverSystem>(this);
    PatrolSystem = NewObject<UPatrolSystem>(this);
    LevelGenerator = Cast<ALevelGenerator>(
            UGameplayStatics::GetActorOfClass(GetWorld(), ALevelGenerator::StaticClass()));

    // Initialise cover and patrol points on completing level generation or immediately if none exist
    if (LevelGenerator && !LevelGenerator->IsGenerationComplete())
    {
        LevelGenerator->OnGenerationComplete.AddDynamic(this, &ALevelGameMode::InitialiseSystems);
    }
    else
    {
        InitialiseSystems();
    }
}

UCoverSystem* ALevelGameMode::GetCoverSystem()
{
    return CoverSystem;
}

UPatrolSystem* ALevelGameMode::GetPatrolSystem()
{
    return PatrolSystem;
}

ALevelGenerator* ALevelGameMode::GetLevelGenerator()
{
    return LevelGenerator;
}

void ALevelGameMode::InitialiseSystems()
{
    // Initialise all UObject systems
    CoverSystem->Initialise();
    PatrolSystem->Initialise();
}