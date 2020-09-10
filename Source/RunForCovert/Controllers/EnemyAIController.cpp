// Caps Collective 2020


#include "EnemyAIController.h"
#include "RunForCovert/Actors/Cover.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/EnemyCharacterBase.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "../Objects/StateMachine.h"
#include "../GameModes/DefaultGameModeBase.h"
#include "RunForCovert/Objects/States/FindCoverState.h"
#include "RunForCovert/Objects/States/PatrolState.h"
#include "RunForCovert/Objects/Transitions/PlayerSeenTransition.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set field default values
    RepeatedAction = 0;
    CoverPoint = nullptr;
    PreviousCoverPoint = nullptr;
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
    PatrolSystem = GameMode->GetPatrolSystem();

    // typedef TPair<UTransition*, UState*> TransitionStatePair;
    // typedef TArray<TransitionStatePair> Transitions;
    // typedef TMap<UState*, Transitions> TransitionMap;
    
    StateMachine = NewObject<UStateMachine>();
    TMap<UState*, TArray<TPair<UTransition*, UState*>>> StateMachineSomething;

    UPatrolState* PatrolState = NewObject<UPatrolState>();
    UFindCoverState* FindCoverState = NewObject<UFindCoverState>();
    UPlayerSeenTransition* PlayerSeenTransition = NewObject<UPlayerSeenTransition>();
    
    StateMachineSomething.Add(NewObject<UPatrolState>(), {
        TPair<UTransition*, UState*>(PlayerSeenTransition, FindCoverState),
    });

    StateMachine->StateTransitions = StateMachineSomething;
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
    StateMachine->OnUpdate(this);

    // TODO there's definitely better ways of doing all this, but this is
    //  just some example code to show how the cover system can be used

    // Uncomment this (and the line in FireAtPlayer) to see the AI constantly run and shoot at the player
//    MoveToActor(Player, 300.f);

    // Uncomment this to see the AI constantly hide from the player
//    if (LineOfSightTo(Player) && !CoverPoint)
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
    // if (CoverPoint)
    // {
    //     if (FVector::Dist(Agent->GetActorLocation(), CoverPoint->GetComponentLocation()) < 100.f)
    //     {
    //         PreviousCoverPoint = CoverPoint;
    //         CoverPoint = nullptr;
    //     }
    // }
    // else if (CoverPath.Num() > 0)
    // {
    //     ACover *Cover = CoverPath.Pop();
    //     if (!Cover->IsOccupiedByOther(Agent))
    //     {
    //         // Find a valid (or failing that, the first) cover point on the cover
    //         UCoverPointComponent *NewCoverPoint = Cover->FindValidCoverPoint(Player->GetActorLocation());
    //         if (!NewCoverPoint) { NewCoverPoint = Cover->GetFirstCover(); }
    //
    //         // Attempt to claim the new cover point, releasing the previous one in the process
    //         if (NewCoverPoint->TrySetOccupation(Agent))
    //         {
    //             CoverPoint = NewCoverPoint;
    //             if (PreviousCoverPoint) { PreviousCoverPoint->ReleaseOccupation(Agent); }
    //             MoveToLocation(CoverPoint->GetComponentLocation());
    //         }
    //     }
    // }
    // else
    // {
    //     CoverPath = CoverSystem->FindCoverPath(Agent, Player);
    // }

    
    // if(PatrolPoint == nullptr)
    // {
    //     PatrolPoint = PatrolSystem->FindClosestValidPatrolPoint(Agent);
    // }
    // else
    // {
    //     if(FVector::Dist(Agent->GetActorLocation(), PatrolPoint->GetActorLocation()) < 100.0f)
    //     {
    //         PatrolPoint = PatrolPoint->AdjacentNodes[FMath::RandRange(0, PatrolPoint->AdjacentNodes.Num()-1)];
    //     }
    //     else
    //     {
    //         MoveToLocation(PatrolPoint->GetActorLocation());    
    //     }
    // }
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