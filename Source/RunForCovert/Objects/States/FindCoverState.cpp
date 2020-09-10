// Caps Collective 2020


#include "FindCoverState.h"
#include "RunForCovert/Actors/Cover.h"
#include "RunForCovert/Objects/CoverSystem.h"


void UFindCoverState::OnEnter(AEnemyAIController& Owner)
{
    UE_LOG(LogTemp, Warning, TEXT("On enter called"))
    if (!Owner.CoverPoint)
    {
        UE_LOG(LogTemp, Warning, TEXT("Setting cover point"))
        // Get the closest valid cover point to the agent
        Owner.CoverPoint = Owner.CoverSystem->FindClosestValidCoverPoint(Owner.Agent, Owner.Player);
    }
}

void UFindCoverState::OnExit(AEnemyAIController& Owner)
{
    Owner.TakenCover = false;
}

void UFindCoverState::OnUpdate(AEnemyAIController& Owner)
{
    if (!Owner.CoverPoint) { return; }
    // Move towards the cover
    Owner.MoveToLocation(Owner.CoverPoint->GetComponentLocation());

    // Nullify the cover point once close enough
    if (FVector::Dist(Owner.Agent->GetActorLocation(), Owner.CoverPoint->GetComponentLocation()) < 1000.f)
    {
        Owner.Agent->SetCrouching(true);
        Owner.CoverPoint = nullptr;
        Owner.TakenCover = true;
    }
}
