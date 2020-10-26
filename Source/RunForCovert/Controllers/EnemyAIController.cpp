// Caps Collective 2020


#include "EnemyAIController.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "RunForCovert/Characters/EnemyCharacterBase.h"
#include "RunForCovert/Characters/PlayerCharacterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../Objects/States/EnemyStateMachine.h"
#include "../GameModes/LevelGameMode.h"


AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    // Setup components
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));

    // Setup the controller's perception and sight config details
    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SightConfig->SightRadius = 1000.0f;
    SightConfig->PeripheralVisionAngleDegrees = 60.0f;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    PerceptionComponent->ConfigureSense(*SightConfig);

    // Set field default values
    Player = nullptr;
    Agent = nullptr;
    PatrolSystem = nullptr;
    StateMachine = nullptr;
    CoverSystem = nullptr;
    SeenPlayerFor = 0.f;
    SenseState = EnemySenseState::DEFAULT;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // TODO this will need to be fixed when dealing with multiple players
    // Get references to player and the controlled agent
    Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    Agent = Cast<AEnemyCharacterBase>(InPawn);

    // Register perception delegate method
    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::SeePlayer);

    // Get a reference to the game mode (used as a service locator)
    ALevelGameMode* GameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
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

    // Smoothly rotate the character based on the controller's rotation
    Agent->SetActorRotation(
            FMath::Lerp(Agent->GetActorRotation(), K2_GetActorRotation(),
                        DeltaTime * Agent->TurningSpeed));

    // If the player has been noticed by the player for TimeToSeePlayer seconds
    // They will flip the boolean changing the state from Patrol to Combat
    if(SenseState == EnemySenseState::PLAYER_SEEN)
    {
        // Start chasing player if they have been seen long enough
        if (SeenPlayerFor > Agent->TimeToSeePlayer)
        {
            SenseState = EnemySenseState::CHASING_PLAYER;
        }
        else
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
    APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(ActorSensed);
    if(PlayerCharacter && Stimulus.WasSuccessfullySensed())
    {
        Player = PlayerCharacter;
        SenseState = EnemySenseState::PLAYER_SEEN;
    }
}