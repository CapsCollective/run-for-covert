// Caps Collective 2020


#include "EngineUtils.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "RunForCovert/Characters/EnemyCharacterBase.h"
#include "RunForCovert/Characters/PlayerCharacterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
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

    // Set up the sight config
    UAISenseConfig_Sight* SightConfig = NewObject<UAISenseConfig_Sight>();
    SightConfig->SightRadius = 1000.0f;
    SightConfig->PeripheralVisionAngleDegrees = 60.0f;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    Agent->PerceptionComponent->ConfigureSense(*SightConfig);

    // Register perception delegate method
    Agent->PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::SeePlayer);

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

void AEnemyAIController::SeePlayer(AActor* ActorSensed, FAIStimulus Stimulus)
{
    // Check bSeenPlayer to true if the detected Stimulus is the player or not
    APlayerCharacterBase* pcb = Cast<APlayerCharacterBase>(ActorSensed);
    if(pcb)
    {
        bSeenPlayer = Stimulus.WasSuccessfullySensed();
    }
}