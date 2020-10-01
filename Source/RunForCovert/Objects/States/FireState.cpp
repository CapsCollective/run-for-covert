// Caps Collective 2020


#include "FireState.h"
#include "HoldCoverState.h"
#include "Engine/World.h"

UFireState::UFireState()
{
    // Set field default values
    TimesToFire = 3;
    TimesFired = 0;
    FireDelay = 1.f;
    TimeStarted = 0.f;
    bHasFinishedFiring = false;
}

void UFireState::OnEnter()
{
    TimeStarted = Owner->GetWorld()->GetTimeSeconds();
    Owner->Agent->SetCrouching(false);
}

void UFireState::OnExit()
{
    bHasFinishedFiring = false;
    TimesFired = 0;
}

void UFireState::OnUpdate()
{
    // Fire at the player once the timer ends
    if (TimeStarted + FireDelay <= Owner->GetWorld()->GetTimeSeconds())
    {
        if (Owner->Agent->FireWeapon() && ++TimesFired >= TimesToFire)
        {
            bHasFinishedFiring = true;
        }
    }
}

UClass* UFireState::ToTransition() const
{
    return bHasFinishedFiring ? UHoldCoverState::StaticClass() : nullptr;
}