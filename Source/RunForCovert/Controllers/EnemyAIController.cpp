// Caps Collective 2020


#include "EnemyAIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}


void AEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}