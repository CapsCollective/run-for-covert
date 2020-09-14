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
    bHasFinishedFiring = false;
    bTakenValidCover = false;
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // TODO this will need to be fixed when dealing with multiple players
    // Get references to player and the controlled agent
    Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    Agent = Cast<AEnemyCharacterBase>(GetPawn());

    // Get a reference to the game mode (used as a service locator)
    ADefaultGameModeBase* GameMode = Cast<ADefaultGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if (!GameMode)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find game mode"))
        return;
    }

    // Populate references to various systems
    CoverSystem = GameMode->GetCoverSystem();
    PatrolSystem = GameMode->GetPatrolSystem();

    // Setup agent state machine
    StateMachine = NewObject<UEnemyStateMachine>();
    StateMachine->Initialise(this);
}


void AEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!(Player && Agent && CoverSystem)) { return; }

    // If the player has been noticed by the player for TimeToSeePlayer seconds
    // They will flip the boolean changing the state from Patrol to Combat
    if(bSeenPlayer)
    {
        // Start chasing player if they have been seen long enough
        if(!(bChasingPlayer = SeenPlayerFor > Agent->TimeToSeePlayer))
        {
            SeenPlayerFor += DeltaTime;
        }
    }
    else if(SeenPlayerFor > 0.0f)
    {
        // Apply cooldown to agent perception of player
        SeenPlayerFor -= DeltaTime;
    }

    // Tick state machine
    StateMachine->OnUpdate();
}