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
    CoverPoint = nullptr;
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // TODO this will need to be fixed when dealing with multiple players
    Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    Agent = Cast<AEnemyCharacterBase>(GetPawn());

    TimerHandle = FTimerHandle(); // TODO this is just an example of how to use timers to call methods, can be removed
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

    // Uncomment this (and the line in FireAtPlayer) to see the AI constantly run and shoot at the player
//    MoveToActor(Player, 300.f);

    // Uncomment this to see the AI constantly hide from the player
//    if (LineOfSightTo(Player) && !CoverPoint) // TODO remember not to use LineOfSightTo as it disregards senses
//    {
//        // Get the closest valid cover point to the agent
//        CoverPoint = CoverSystem->FindClosestValidCoverPoint(Agent, Player);
//    }
//    else if (CoverPoint)
//    {
//        // Move towards the player while there is a valid cover point
//        MoveToLocation(CoverPoint->GetComponentLocation());
//
//        // Nullify the cover point once close enough
//        if (FVector::Dist(Agent->GetActorLocation(), CoverPoint->GetComponentLocation()) < 1000.f)
//        {
//            CoverPoint = nullptr;
//        }
//    }

    // Uncomment this to see the AI path towards the nearest cover to the player
    if (CoverPath.Num() <= 0 && !CoverPoint)
    {
        CoverPath = CoverSystem->FindCoverPath(Agent, Player);
    }
    else if (!CoverPoint)
    {
        ACover* Cover = CoverPath.Pop();
        if (!(CoverPoint = Cover->FindValidCoverPoint(Player->GetActorLocation())))
        {
            // TODO there's definitely better ways of doing this so they don't
            //  randomly oscillate between the two sides of their final cover point,
            //  this is just some example code to show how the cover system can be used
            CoverPoint = Cover->GetRandomCover();
        }
    }
    else
    {
        MoveToLocation(CoverPoint->GetComponentLocation());
        if (FVector::Dist(Agent->GetActorLocation(), CoverPoint->GetComponentLocation()) < 100.f)
        {
            CoverPoint = nullptr;
        }
    }
}

void AEnemyAIController::FireAtPlayer()
{
    if (!Agent) { return; }

    // TODO make the AI face and aim at the player when firing
    // Uncomment this (and the line in Tick) to see the AI constantly run and shoot at the player
//    Agent->FireWeapon();

    if (++RepeatedAction >= 15)
    {
        GetWorldTimerManager().ClearTimer(TimerHandle);
    }
}