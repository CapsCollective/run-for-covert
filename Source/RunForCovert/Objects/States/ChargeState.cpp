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
    Owner->MoveToActor(Owner->Player);
}

void UChargeState::OnExit()
{
    bCharged = false;
}

void UChargeState::OnUpdate()
{
    // Have the agent fire their weapon while charging
    Owner->Agent->FireWeapon();

    // End the charge once close enough to the player
    if (FVector::Dist(Owner->Agent->GetActorLocation(), Owner->Player->GetActorLocation()) < 300.f)
    {
        bCharged = true;
    }
}

UClass* UChargeState::ToTransition() const
{
    return bCharged ? UMoveCoverState::StaticClass() : nullptr;
}