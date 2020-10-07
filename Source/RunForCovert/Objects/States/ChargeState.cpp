// Caps Collective 2020


#include "ChargeState.h"
#include "MoveCoverState.h"


UChargeState::UChargeState()
{
    // Set field default values
    bCharged = false;
}

void UChargeState::OnEnter()
{
    // Exit agent crouching and start moving towards the player
    Owner->Agent->SetCrouching(false);
}

void UChargeState::OnExit()
{
    bCharged = false;
}

void UChargeState::OnUpdate()
{
    // Move towards the player
    Owner->MoveToActor(Owner->Player);

    // Calculate distance to player
    float DistanceToPlayer = FVector::Dist(Owner->Agent->GetActorLocation(), Owner->Player->GetActorLocation());
    if (DistanceToPlayer < 300.f)
    {
        // End the charge once close enough to the player
        bCharged = true;
    }
    else if (DistanceToPlayer < Owner->Agent->FiringRange && Owner->LineOfSightTo(Owner->Player))
    {
        // Have the agent fire their weapon while charging if they can see the player and are near enough
        Owner->Agent->FireWeapon();
    }
}

UClass* UChargeState::ToTransition() const
{
    return bCharged ? UMoveCoverState::StaticClass() : nullptr;
}