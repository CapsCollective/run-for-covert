// Caps Collective 2020


#include "PatrolState.h"
#include "CombatStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RunForCovert/Actors/Patrol.h"
#include "RunForCovert/Objects/PatrolSystem.h"

// When the state is "exited" the speed of the player is doubled to "run"
void UPatrolState::OnExit(AEnemyAIController& Owner)
{
    Owner.Agent->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

// Called every tick by the state machine
void UPatrolState::OnUpdate(AEnemyAIController& Owner)
{
    // if no PatrolPoint is found find the closes one
    if(!PatrolPoint)
    {
        PatrolPoint = Owner.PatrolSystem->FindClosestValidPatrolPoint(Owner.Agent);
    }
    else
    {
        // If the distance from the Actor is less than 100 units select the next one in the list as the next patrol point
        if(FVector::Dist(Owner.Agent->GetActorLocation(), PatrolPoint->GetActorLocation()) < 100.0f)
        {
            PatrolPoint = PatrolPoint->AdjacentNodes[FMath::RandRange(0, PatrolPoint->AdjacentNodes.Num()-1)];
        }
        else
        {
            // If the player isn't seen, path to the PatrolPoint
            if(!Owner.Agent->bSeePlayer)
            {
                Owner.MoveToLocation(PatrolPoint->GetActorLocation());
            }
            // If the player IS seen, stop in place
            else
            {
                Owner.MoveToLocation(Owner.Agent->GetActorLocation());
            }   
        }
    }
}

// Transition handling for the AI
UClass* UPatrolState::ToTransition(AEnemyAIController& Owner) const
{
    return Owner.Agent->bChasePlayer ? UCombatStateMachine::StaticClass() : nullptr;
}
