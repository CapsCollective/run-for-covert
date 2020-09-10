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
    CoverPoint = nullptr;
    PreviousCoverPoint = nullptr;
    HasFired = false;
}

bool AEnemyAIController::bPlayerSeen()
{
    return Agent->bChasePlayer;
}

bool AEnemyAIController::bSeePlayer()
{
    return Agent->bSeePlayer;
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


//  TODO implement desired AI behaviours:
//      - Sensing
//      - Patrolling
//      - Investigating
//      - Combat
//
//  TODO Potential combat behaviour:
//      1) On alert, run to nearest valid cover ("valid" meaning away from player line of sight)
//      2) Generate cover path toward player
//      3) Fire from cover location while cover is valid
//      4) Check if next cover point in path is valid
//          4.a) If so, advance to next cover point
//          4.b) Else generate new valid path and move
//      5) If the player gets line of sight while agent takes cover, go to step 1


void AEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!(Player && Agent && CoverSystem)) { return; }
    StateMachine->OnUpdate(*this);
}

void AEnemyAIController::FireAtPlayer()
{
    if (!Agent) { return; }
    Agent->FireWeapon();
}