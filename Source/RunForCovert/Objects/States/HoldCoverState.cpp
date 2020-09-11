// Caps Collective 2020


#include "HoldCoverState.h"
#include "FireState.h"
#include "MoveCoverState.h"

UHoldCoverState::UHoldCoverState()
{
    // Set field default values
    TimeToCover = 1.5f;
    TimeStarted = 0.f;
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
    Owner->Agent->SetCrouching(false);
}

void UHoldCoverState::OnUpdate()
{
    // Nullify cover validity once the timer ends
    if (TimeStarted + TimeToCover <= Owner->GetWorld()->GetTimeSeconds())
    {
        Owner->TakenValidCover = false;
    }
}

UClass* UHoldCoverState::ToTransition() const
{
    if (!Owner->TakenValidCover && Owner->Agent->GetDistanceTo(Owner->Player) < Owner->Agent->FiringRange)
    {
        return UFireState::StaticClass();
    }
    else if (!Owner->TakenValidCover)
    {
        return UMoveCoverState::StaticClass();
    }
    return nullptr;
}