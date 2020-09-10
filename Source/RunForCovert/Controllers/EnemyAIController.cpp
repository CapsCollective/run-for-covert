// Caps Collective 2020


#include "EnemyAIController.h"
#include "RunForCovert/Actors/Cover.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "../Objects/States/EnemyStateMachine.h"
#include "../GameModes/DefaultGameModeBase.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set field default values
    HasFired = false;
    TakenValidCover = false;
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // TODO this will need to be fixed when dealing with multiple players
    Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    Agent = Cast<AEnemyCharacterBase>(GetPawn());

    // Get a reference to the game mode (used as a service locator)
    ADefaultGameModeBase* GameMode = Cast<ADefaultGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if (!GameMode)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find game mode"))
        return;
    }
    CoverSystem = GameMode->GetCoverSystem();
    PatrolSystem = GameMode->GetPatrolSystem();
    
    StateMachine = NewObject<UEnemyStateMachine>();
    StateMachine->Initialise();
}


void AEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!(Player && Agent && CoverSystem)) { return; }
    StateMachine->OnUpdate(*this);
}