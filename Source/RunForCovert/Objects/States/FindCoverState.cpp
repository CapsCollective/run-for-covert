// Caps Collective 2020


#include "FindCoverState.h"


#include "RunForCovert/Actors/Cover.h"
#include "RunForCovert/Objects/CoverSystem.h"

void UFindCoverState::OnEnter()
{
}

void UFindCoverState::OnExit()
{
}

void UFindCoverState::OnUpdate(AEnemyAIController* Owner)
{
    // OnEnter
    CoverPoint = Owner->CoverSystem->FindClosestValidCoverPoint(Owner->Agent, Owner->Player);

    if (CoverPoint)
    {
        // Move towards the player while there is a valid cover point
        Owner->MoveToLocation(CoverPoint->GetComponentLocation());

        // Nullify the cover point once close enough
        if (FVector::Dist(Owner->Agent->GetActorLocation(), CoverPoint->GetComponentLocation()) < 1000.f)
        {
            CoverPoint = nullptr;
        }
    }

//        // Uncomment this to see the AI path towards the nearest cover to the player
//        if (CoverPoint)
//        {
//            if (FVector::Dist(Owner->Agent->GetActorLocation(), CoverPoint->GetComponentLocation()) < 100.f)
//            {
//                PreviousCoverPoint = CoverPoint;
//                CoverPoint = nullptr;
//            }
//        }
//        else if (CoverPath.Num() > 0)
//        {
//            ACover *Cover = CoverPath.Pop();
//            if (!Cover->IsOccupiedByOther(Owner->Agent))
//            {
//                // Find a valid (or failing that, the first) cover point on the cover
//                UCoverPointComponent *NewCoverPoint = Cover->FindValidCoverPoint(Owner->Player->GetActorLocation());
//                if (!NewCoverPoint) { NewCoverPoint = Cover->GetFirstCover(); }
//
//                // Attempt to claim the new cover point, releasing the previous one in the process
//                if (NewCoverPoint->TrySetOccupation(Owner->Agent))
//                {
//                    CoverPoint = NewCoverPoint;
//                    if (PreviousCoverPoint) { PreviousCoverPoint->ReleaseOccupation(Owner->Agent); }
//                    Owner->MoveToLocation(CoverPoint->GetComponentLocation());
//                }
//            }
//        }
//        else
//        {
//            CoverPath = Owner->CoverSystem->FindCoverPath(Owner->Agent, Owner->Player);
//        }
}
