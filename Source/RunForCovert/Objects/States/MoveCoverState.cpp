// Caps Collective 2020


#include "MoveCoverState.h"
#include "RunForCovert/Actors/Cover.h"
#include "RunForCovert/Objects/CoverSystem.h"
#include "HoldCoverState.h"
#include "ChargeState.h"

UMoveCoverState::UMoveCoverState()
{
    // Set field default values
    bTakenFinalCover = false;
    CoverPoint = nullptr;
    PreviousCoverPoint = nullptr;
}

void UMoveCoverState::OnEnter()
{
    // Exit agent crouching
    if (!Owner) { return; }
    Owner->Agent->SetCrouching(false);
}

void UMoveCoverState::OnExit()
{
    bTakenFinalCover = false;
}

void UMoveCoverState::OnUpdate()
{
    if (CoverPoint)
    {
        // Check if the agent has arrived at the current cover point
        if (FVector::Dist(Owner->Agent->GetActorLocation(), CoverPoint->GetComponentLocation()) < 100.f)
        {
            PreviousCoverPoint = CoverPoint;
            CoverPoint = nullptr;

            // Exit the state if the end of the path has been reached
            if (CoverPath.Num() == 0)
            {
                bTakenFinalCover = true;
            }
        }
    }
    else if (CoverPath.Num() > 0)
    {
        // Check a potential new cover actor
        ACover *Cover = CoverPath.Pop();
        if (!Cover->IsOccupiedByOther(Owner->Agent))
        {
            // Find a valid (or failing that, the first) cover point on the cover
            UCoverPointComponent *NewCoverPoint = Cover->FindValidCoverPoint(Owner->Player->GetActorLocation());
            if (!NewCoverPoint) { NewCoverPoint = Cover->GetFirstCover(); }

            // Attempt to claim the new cover point, releasing the previous one in the process
            if (NewCoverPoint->TrySetOccupation(Owner->Agent))
            {
                CoverPoint = NewCoverPoint;
                if (PreviousCoverPoint) { PreviousCoverPoint->ReleaseOccupation(Owner->Agent); }
                Owner->MoveToLocation(CoverPoint->GetComponentLocation());
            }
        }
    }
    else
    {
        // Generate a new path towards the player
        CoverPath = Owner->CoverSystem->FindCoverPath(Owner->Agent, Owner->Player);
    }
}

UClass* UMoveCoverState::ToTransition() const
{
    bool bWithinFiringRange = Owner->Agent->GetDistanceTo(Owner->Player) < Owner->Agent->FiringRange;

    if (bTakenFinalCover && bWithinFiringRange)
    {
        return UHoldCoverState::StaticClass();
    }
    else if (bTakenFinalCover && !bWithinFiringRange)
    {
        return UChargeState::StaticClass();
    }
    return nullptr;
}