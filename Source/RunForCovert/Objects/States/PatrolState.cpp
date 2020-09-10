// Caps Collective 2020


#include "PatrolState.h"


#include "RunForCovert/Actors/Patrol.h"
#include "RunForCovert/Objects/PatrolSystem.h"

void UPatrolState::OnEnter(AEnemyAIController& Owner)
{
}

void UPatrolState::OnExit(AEnemyAIController& Owner)
{

}

void UPatrolState::OnUpdate(AEnemyAIController& Owner)
{
    if(!PatrolPoint)
    {
        PatrolPoint = Owner.PatrolSystem->FindClosestValidPatrolPoint(Owner.Agent);
    }
    else
    {
        if(FVector::Dist(Owner.Agent->GetActorLocation(), PatrolPoint->GetActorLocation()) < 100.0f)
        {
            PatrolPoint = PatrolPoint->AdjacentNodes[FMath::RandRange(0, PatrolPoint->AdjacentNodes.Num()-1)];
        }
        else
        {
            if(!Owner.bSeePlayer())
            {
                Owner.MoveToLocation(PatrolPoint->GetActorLocation());
            }
            else
            {
                Owner.MoveToLocation(Owner.Agent->GetActorLocation());
            }   
        }
    }
}