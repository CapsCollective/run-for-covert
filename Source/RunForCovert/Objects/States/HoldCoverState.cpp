// Caps Collective 2020


#include "HoldCoverState.h"
#include "FireState.h"
#include "MoveCoverState.h"
#include "Engine/World.h"

UHoldCoverState::UHoldCoverState()
{
    // Set field default values
    TimeToCover = 1.5f;
    TimeStarted = 0.f;
    bHeldCover = false;
}

void UHoldCoverState::OnEnter()
{
    // Initiate agent crouching and start the timer
    Owner->Agent->SetCrouching(true);
    TimeStarted = Owner->GetWorld()->GetTimeSeconds();
}

void UHoldCoverState::OnExit()
{
    // Reset crouching
    bHeldCover = false;
}

void UHoldCoverState::OnUpdate()
{
    // Set the flag once the timer ends
    if (TimeStarted + TimeToCover <= Owner->GetWorld()->GetTimeSeconds())
    {
        bHeldCover = true;
    }
}

UClass* UHoldCoverState::ToTransition() const
{
    if (bHeldCover && Owner->Agent->GetDistanceTo(Owner->Player) < Owner->Agent->FiringRange)
    {
        return UFireState::StaticClass();
    }
    else if (bHeldCover)
    {
        return UMoveCoverState::StaticClass();
    }
    return nullptr;
}