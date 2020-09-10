// Caps Collective 2020


#include "MoveCoverState.h"
#include "RunForCovert/Actors/Cover.h"
#include "RunForCovert/Objects/CoverSystem.h"

void UMoveCoverState::OnEnter(AEnemyAIController& Owner)
{
}

void UMoveCoverState::OnExit(AEnemyAIController& Owner)
{
}

void UMoveCoverState::OnUpdate(AEnemyAIController& Owner)
{
    if (Owner.CoverPoint)
    {
        if (FVector::Dist(Owner.Agent->GetActorLocation(), Owner.CoverPoint->GetComponentLocation()) < 100.f)
        {
            Owner.PreviousCoverPoint = Owner.CoverPoint;
            Owner.CoverPoint = nullptr;
        }
    }
    else if (Owner.CoverPath.Num() > 0)
    {
        ACover *Cover = Owner.CoverPath.Pop();
        if (!Cover->IsOccupiedByOther(Owner.Agent))
        {
            // Find a valid (or failing that, the first) cover point on the cover
            UCoverPointComponent *NewCoverPoint = Cover->FindValidCoverPoint(Owner.Player->GetActorLocation());
            if (!NewCoverPoint) { NewCoverPoint = Cover->GetFirstCover(); }

            // Attempt to claim the new cover point, releasing the previous one in the process
            if (NewCoverPoint->TrySetOccupation(Owner.Agent))
            {
                Owner.CoverPoint = NewCoverPoint;
                if (Owner.PreviousCoverPoint) { Owner.PreviousCoverPoint->ReleaseOccupation(Owner.Agent); }
                Owner.MoveToLocation(Owner.CoverPoint->GetComponentLocation());
            }
        }
    }
    else
    {
        Owner.CoverPath = Owner.CoverSystem->FindCoverPath(Owner.Agent, Owner.Player);
    }
}