// Caps Collective 2020


#include "MoveCoverState.h"
#include "RunForCovert/Actors/Cover.h"
#include "RunForCovert/Objects/CoverSystem.h"
#include "HoldCoverState.h"

void UMoveCoverState::OnUpdate(AEnemyAIController& Owner)
{
    if (CoverPoint)
    {
        // Check if the agent has arrived at the current cover point
        if (FVector::Dist(Owner.Agent->GetActorLocation(), CoverPoint->GetComponentLocation()) < 100.f)
        {
            // Flag that a valid cover position has been taken if it provides cover
            if (CoverPoint->DoesProvideCover(Owner.Player->GetActorLocation()))
            {
                Owner.TakenValidCover = true;
            }
            PreviousCoverPoint = CoverPoint;
            CoverPoint = nullptr;
        }
    }
    else if (CoverPath.Num() > 0)
    {
        // Check a potential new cover actor
        ACover *Cover = CoverPath.Pop();
        if (!Cover->IsOccupiedByOther(Owner.Agent))
        {
            // Find a valid (or failing that, the first) cover point on the cover
            UCoverPointComponent *NewCoverPoint = Cover->FindValidCoverPoint(Owner.Player->GetActorLocation());
            if (!NewCoverPoint) { NewCoverPoint = Cover->GetFirstCover(); }

            // Attempt to claim the new cover point, releasing the previous one in the process
            if (NewCoverPoint->TrySetOccupation(Owner.Agent))
            {
                CoverPoint = NewCoverPoint;
                if (PreviousCoverPoint) { PreviousCoverPoint->ReleaseOccupation(Owner.Agent); }
                Owner.MoveToLocation(CoverPoint->GetComponentLocation());
            }
        }
    }
    else
    {
        // Generate a new path towards the player
        CoverPath = Owner.CoverSystem->FindCoverPath(Owner.Agent, Owner.Player);
    }
}

UClass* UMoveCoverState::ToTransition(AEnemyAIController& Owner) const
{
    return Owner.TakenValidCover ? UHoldCoverState::StaticClass() : nullptr;
}