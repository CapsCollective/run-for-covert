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

    Character = Cast<AEnemyCharacterBase>(GetPawn());

    TimerHandle = FTimerHandle();
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyAIController::FireAtPlayer,
                                    1.0f, true, 2.0f);
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

    //MoveToActor(Player, 300.f);

    // Uncomment this (and comment out the above) to test the cover system
    if (LineOfSightTo(Player) && !CoverPosition) // TODO remember not to use LineOfSightTo as it disregards senses
    {
        // Get a reference to the game mode (used as a service locator)
        ACoverSystem* CoverSystem;
        ADefaultGameModeBase* GameMode = Cast<ADefaultGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

        if (!GameMode || !(CoverSystem = GameMode->GetCoverSystem())) { return; }

        // Get the closest valid cover point to the agent
        CoverPosition = CoverSystem->GetClosestValidCoverPoint(Character, Player);
    }
    else if (CoverPosition)
    {
        // Move towards the player while there is a valid cover point
        MoveToLocation(CoverPosition->GetComponentLocation());

        // Nullify the cover point once close enough
        if (FVector::Dist(Character->GetActorLocation(), CoverPosition->GetComponentLocation()) < 1000.f)
        {
            CoverPosition = nullptr;
        }
    }
}

void AEnemyAIController::FireAtPlayer()
{
    if (!Character) { return; }

    //Character->FireWeapon();

    if (++RepeatedAction >= 15)
    {
        GetWorldTimerManager().ClearTimer(TimerHandle);
    }
}