// Caps Collective 2020


#include "PatrolState.h"
#include "CombatStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RunForCovert/Actors/Patrol.h"
#include "RunForCovert/Objects/PatrolSystem.h"


void UPatrolState::OnExit()
{
    Owner->Agent->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void UPatrolState::OnUpdate()
{
    // if no PatrolPoint is found find the closes one
    if(!PatrolPoint)
    {
        PatrolPoint = Owner->PatrolSystem->FindClosestValidPatrolPoint(Owner->Agent);
    }
    else
    {
        if(FVector::Dist(Owner->Agent->GetActorLocation(), PatrolPoint->GetActorLocation()) < 100.0f)
        {
            PatrolPoint = PatrolPoint->AdjacentNodes[FMath::RandRange(0, PatrolPoint->AdjacentNodes.Num()-1)];
        }
        else
        {
            if(!Owner->Agent->bSeePlayer)
            {
                Owner->MoveToLocation(PatrolPoint->GetActorLocation());
            }
            // If the player IS seen, stop in place
            else
            {
                Owner->MoveToLocation(Owner->Agent->GetActorLocation());
            }   
        }
    }
}

UClass* UPatrolState::ToTransition() const
{
    return Owner->Agent->bChasePlayer ? UCombatStateMachine::StaticClass() : nullptr;
}
