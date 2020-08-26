// Caps Collective 2020


#include "CoverSystem.h"
#include "EngineUtils.h"
#include "../Actors/Cover.h"
#include "DrawDebugHelpers.h"

#define OUT

ACoverSystem::ACoverSystem()
{
	PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    CoverRadius = 1000.f;
}

void ACoverSystem::BeginPlay()
{
	Super::BeginPlay();

	// Get a list of all cover actors in the world and generate a list of
	// nodes representing cover actors with graph attributes
    for (ACover* Cover : TActorRange<ACover>(GetWorld()))
    {
        // Add to cover list
        CoverActors.Add(Cover);

        // Add cover node list
        UCoverNode* NewNode = NewObject<UCoverNode>();
        NewNode->CoverActor = Cover;
        CoverNodes.Add(NewNode);
    }

    if (CoverActors.Num() < 3) {
        UE_LOG(LogTemp, Error, TEXT("Cannot generate cover system for less than three actors"))
        return;
    }

    // Generate a random geometric graph of R depth between cover items
    for (UCoverNode* CurrentNode : CoverNodes)
    {
        for (UCoverNode* Node : CoverNodes)
        {
            if (Node->CoverActor->GetDistanceTo(CurrentNode->CoverActor) <= CoverRadius && Node->CoverActor != CurrentNode->CoverActor)
            {
                CurrentNode->AdjacentCover.Add(Node);
            }
        }
    }

    // Display debug lines
    for (UCoverNode* Node : CoverNodes)
    {
        for (UCoverNode* ConnectedNode : Node->AdjacentCover)
        {
            DrawDebugLine(GetWorld(), Node->CoverActor->GetActorLocation(),
                          ConnectedNode->CoverActor->GetActorLocation(), FColor::Green, true);
        }
    }
}

UCoverPositionComponent* ACoverSystem::GetClosestValidCoverPoint(AActor* Agent, AActor* Enemy)
{
    // Iterate through all cover points in world
    float ClosestCoverDistance = TNumericLimits<float>::Max();
    UCoverPositionComponent* ClosestCoverPoint = nullptr;
    for (auto It = CoverActors.CreateConstIterator(); It; It++)
    {
        // Check if the current point is closer than
        float CoverDistance = (*It)->GetDistanceTo(Agent);
        if (CoverDistance >= ClosestCoverDistance) { continue; }

        // Check if it actually provides cover from the enemy
        UCoverPositionComponent* PotentialCover = (*It)->FindCover(Enemy->GetActorLocation());
        if (PotentialCover)
        {
            ClosestCoverDistance = CoverDistance;
            ClosestCoverPoint = PotentialCover;
        }
    }
    return ClosestCoverPoint;
}

TArray<UCoverPositionComponent*> ACoverSystem::GetCoverPath(UCoverPositionComponent* CurrentCoverPosition, AActor* Enemy)
{
    // TODO implement path generation of valid cover points toward the enemy

//    TArray<FCoverNode> OpenSet;
//    FCoverNode StartNode;
//    FCoverNode EndNode; // TODO calculate this
//
//    for (ACover* Cover : CoverActors)
//    {
//        FCoverNode NewNode = FCoverNode();
//        NewNode.GScore = TNumericLimits<float>::Max();
//
//        if (Cover == CurrentCoverPosition->GetOwner())
//        {
//            StartNode = NewNode;
//        }
//    }
//
//    StartNode.GScore = 0.f;
//    StartNode.HScore = StartNode.CoverActor->GetOwner()->GetDistanceTo(EndNode.CoverActor->GetOwner());
//
//    OpenSet.Add(StartNode);
//
//    while (OpenSet.Num() > 0)
//    {
//        OpenSet.Sort([](FCoverNode &A, FCoverNode &B){
//            return A.FScore() < B.FScore();
//        });
//        FCoverNode CurrentNode = OpenSet.Pop();
//
//        if (CurrentNode.CoverActor == EndNode.CoverActor) {
//            TArray<UCoverPositionComponent*> Path;
//            Path.Push(EndNode.ValidCover);
//            CurrentNode = EndNode;
//            while (CurrentNode.CoverActor != StartNode.CoverActor)
//            {
//                CurrentNode = *CurrentNode.CameFrom;
//                Path.Add(CurrentNode.ValidCover);
//            }
//            return Path;
//        }
//
//        for (FCoverNode* ConnectedNode : CurrentNode.ConnectedNodes)
//        {
//            float TentativeGScore = CurrentNode.GScore +
//            FVector::Distance(CurrentNode.CoverActor->GetActorLocation(), ConnectedNode->GetActorLocation());
//            if (TentativeGScore < ConnectedNode.GScore)
//            {
//                ConnectedNode->CameFrom = CurrentNode;
//                ConnectedNode->GScore = TentativeGScore;
//                ConnectedNode->HScore = FVector::Distance(ConnectedNode->GetActorLocation(), EndNode->GetActorLocation());
//                if (!OpenSet.Contains(ConnectedNode))
//                {
//                    OpenSet.Add(ConnectedNode);
//                }
//            }
//        }
//    }

    //If it leaves this loop without finding the end node then return an empty path.
    UE_LOG(LogTemp, Error, TEXT("NO PATH FOUND"));
    return TArray<UCoverPositionComponent*>();
}