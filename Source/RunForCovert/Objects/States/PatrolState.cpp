// Caps Collective 2020


#include "PatrolState.h"
#include "CombatStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RunForCovert/Actors/Patrol.h"
#include "RunForCovert/Objects/PatrolSystem.h"

void UPatrolState::OnExit()
{
    // When the state is exited, set the speed of the agent to 600
    Owner->Agent->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void UPatrolState::OnUpdate()
{
    if(!PatrolPoint)
    {
        // Find the closes patrol point if null
        PatrolPoint = Owner->PatrolSystem->FindClosestPatrolPoint(Owner->Agent);
    }
    else if(FVector::Dist(Owner->Agent->GetActorLocation(), PatrolPoint->GetActorLocation()) < 100.0f)
    {
        // Select a random adjacent node as next target
        PatrolPoint = PatrolPoint->AdjacentNodes[FMath::RandRange(0, PatrolPoint->AdjacentNodes.Num()-1)];
    }
    else if(!Owner->Agent->bSeenPlayer)
    {
        // Move to the following patrol point
        Owner->MoveToLocation(PatrolPoint->GetActorLocation());
    }
    else
    {
        // Stop in place if the player is seen
        Owner->StopMovement();
    }
}

UClass* UPatrolState::ToTransition() const
{
    return Owner->Agent->bChasingPlayer ? UCombatStateMachine::StaticClass() : nullptr;
}
