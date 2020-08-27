// Caps Collective 2020


#include "EnemyAIController.h"
#include "RunForCovert/Actors/Cover.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/EnemyCharacterBase.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "../GameModes/DefaultGameModeBase.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    RepeatedAction = 0;
    CoverPosition = nullptr;
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    Agent = Cast<AEnemyCharacterBase>(GetPawn());

    TimerHandle = FTimerHandle();
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyAIController::FireAtPlayer,
                                    1.0f, true, 2.0f);

    // Get a reference to the game mode (used as a service locator)
    ADefaultGameModeBase* GameMode = Cast<ADefaultGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if (!GameMode)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find game mode"))
        return;
    }
    CoverSystem = GameMode->GetCoverSystem();
}


//  TODO implement desired AI behaviours:
//      - Sensing
//      - Patrolling
//      - Investigating
//      - Combat
//
//  Potential combat behaviour:
//      1) On alert, run to nearest valid cover ("valid" meaning away from player line of sight)
//      2) Generate valid cover-based path toward player
//      3) Fire from cover location while cover is valid
//      4) Check if next cover point is still valid
//          4.a) If so, advance to next cover point
//          4.b) Else generate new valid path and move


void AEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!Player) { return; }

    // Uncomment this (and the line in FireAtPlayer) to see the AI constantly run and shoot at the player
    //MoveToActor(Player, 300.f);

    // Uncomment this to see the AI constantly hide from the player
//    if (LineOfSightTo(Player) && !CoverPosition) // TODO remember not to use LineOfSightTo as it disregards senses
//    {
//        // Get the closest valid cover point to the agent
//        CoverPosition = CoverSystem->GetClosestValidCoverPoint(Agent, Player);
//    }
//    else if (CoverPosition)
//    {
//        // Move towards the player while there is a valid cover point
//        MoveToLocation(CoverPosition->GetComponentLocation());
//
//        // Nullify the cover point once close enough
//        if (FVector::Dist(Agent->GetActorLocation(), CoverPosition->GetComponentLocation()) < 1000.f)
//        {
//            CoverPosition = nullptr;
//        }
//    }

    // Uncomment this to see the AI path towards the nearest cover to the player
    if (CoverPath.Num() <= 0 && !CoverPosition)
    {
        CoverPath = CoverSystem->GetCoverPath(Agent, Player);
    }
    else if (!CoverPosition)
    {
        CoverPosition = CoverPath.Pop()->GetRandomCover();
    }
    else
    {
        MoveToLocation(CoverPosition->GetComponentLocation());
        if (FVector::Dist(Agent->GetActorLocation(), CoverPosition->GetComponentLocation()) < 100.f)
        {
            CoverPosition = nullptr;
        }
    }
}

void AEnemyAIController::FireAtPlayer()
{
    if (!Agent) { return; }

    // Uncomment this (and the line in Tick) to see the AI constantly run and shoot at the player
    //Character->FireWeapon();

    if (++RepeatedAction >= 15)
    {
        GetWorldTimerManager().ClearTimer(TimerHandle);
    }
}