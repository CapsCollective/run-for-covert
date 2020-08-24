// Caps Collective 2020


#include "RunForCovert/Actors/Cover.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/EnemyCharacterBase.h"
#include "TimerManager.h"
#include "EngineUtils.h"

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


void AEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!Player) { return; }

    MoveToActor(Player, 300.f);

    // Uncomment this (and comment out the above) to test the cover system
//    if (LineOfSightTo(Player) && !CoverPosition)
//    {
//        FVector PlayerLocation = Player->GetActorLocation();
//
//        TArray<AActor*> IgnoredActors;
//        IgnoredActors.Add(GetPawn());
//        IgnoredActors.Add(Player);
//
//        UCoverPositionComponent* PotentialCover;
//        for (ACover* Cover : TActorRange<ACover>(GetWorld()))
//        {
//            PotentialCover = Cover->FindCover(PlayerLocation, IgnoredActors);
//            if (PotentialCover) {
//                CoverPosition = PotentialCover;
//                break;
//            }
//        }
//    }
//    else if (CoverPosition)
//    {
//        MoveToLocation(CoverPosition->GetComponentLocation());
//
//        if (FVector::Dist(Character->GetActorLocation(), CoverPosition->GetComponentLocation()) < 1000.f)
//        {
//            CoverPosition = nullptr;
//        }
//    }
}

void AEnemyAIController::FireAtPlayer()
{
    if (!Character) { return; }

    Character->FireWeapon();

    if (++RepeatedAction >= 15)
    {
        GetWorldTimerManager().ClearTimer(TimerHandle);
    }
}