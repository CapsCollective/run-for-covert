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

	// Populate a list of nodes representing all cover actors in the
	// world wrapped in objects specifying graph attributes
    for (ACover* Cover : TActorRange<ACover>(GetWorld()))
    {
        // Add cover node list
        UCoverNode* NewNode = NewObject<UCoverNode>();
        NewNode->CoverActor = Cover;
        CoverNodes.Add(NewNode);
    }

    // Generate the graph connections
    GenerateGraph(CoverRadius);
    DisplayDebugGraph(2.f);
}

UCoverPositionComponent* ACoverSystem::GetClosestValidCoverPoint(AActor* Agent, AActor* Enemy)
{
    // Iterate through all cover points in world
    float ClosestCoverDistance = TNumericLimits<float>::Max();
    UCoverPositionComponent* ClosestCoverPoint = nullptr;
    for (auto It = CoverNodes.CreateConstIterator(); It; It++)
    {
        // Check if the current point is closer than stored
        float CoverDistance = (*It)->CoverActor->GetDistanceTo(Agent);
        if (CoverDistance >= ClosestCoverDistance) { continue; }

        // Check if it actually provides cover from the enemy
        UCoverPositionComponent* PotentialCover = (*It)->CoverActor->FindCover(Enemy->GetActorLocation());
        if (PotentialCover)
        {
            ClosestCoverDistance = CoverDistance;
            ClosestCoverPoint = PotentialCover;
        }
    }
    return ClosestCoverPoint;
}

TArray<ACover*> ACoverSystem::GetCoverPath(AActor* Agent, AActor* Enemy)
{
    // Set up the search for values
    TArray<UCoverNode*> OpenSet;
    UCoverNode* StartNode = GetClosestCover(Agent);
    UCoverNode* EndNode = GetClosestCover(Enemy);
    assert(StartNode && EndNode);

    // Reset the GScore of all nodes in the collection
    for (auto It = CoverNodes.CreateConstIterator(); It; It++)
    {
        (*It)->GScore = TNumericLimits<float>::Max();
    }

    // Setup start node and add to open set
    StartNode->GScore = 0.f;
    StartNode->HScore = StartNode->CoverActor->GetDistanceTo(EndNode->CoverActor);
    OpenSet.Add(StartNode);

    // Iterate through open set while the collection contains items
    while (OpenSet.Num() > 0)
    {
        // Get the node with the lowest FScore
        OpenSet.Sort([](UCoverNode &A, UCoverNode &B){ return A.FScore() < B.FScore(); });
        UCoverNode* CurrentNode = OpenSet.Pop();

        // Once the end node is reached, generate and return a path
        if (CurrentNode == EndNode) {
            TArray<ACover*> Path;
            Path.Push(EndNode->CoverActor);
            CurrentNode = EndNode;
            while (CurrentNode != StartNode)
            {
                CurrentNode = CurrentNode->CameFrom;
                Path.Add(CurrentNode->CoverActor);
            }
            DisplayDebugPath(&Path, .5f);
            return Path;
        }

        // Explore each cover node adjacent to current
        for (UCoverNode* ConnectedNode : CurrentNode->AdjacentCover)
        {
            // Calculate tentative node GScore and check if it's better than the node it comes from
            float TentativeGScore = CurrentNode->GScore + CurrentNode->CoverActor->GetDistanceTo(ConnectedNode->CoverActor);
            if (TentativeGScore < ConnectedNode->GScore)
            {
                // Set scores and add to open set
                ConnectedNode->CameFrom = CurrentNode;
                ConnectedNode->GScore = TentativeGScore;
                ConnectedNode->HScore = ConnectedNode->CoverActor->GetDistanceTo(EndNode->CoverActor);
                if (!OpenSet.Contains(ConnectedNode))
                {
                    OpenSet.Add(ConnectedNode);
                }
            }
        }
    }
    // Return empty if no valid path is found
    return TArray<ACover*>();
}

UCoverNode* ACoverSystem::GetClosestCover(AActor* Actor)
{
    // Iterate through all cover points in world
    float ClosestCoverDistance = TNumericLimits<float>::Max();
    UCoverNode* ClosestCover = nullptr;
    for (auto It = CoverNodes.CreateConstIterator(); It; It++)
    {
        // Check if the current point is closer
        float CoverDistance = (*It)->CoverActor->GetDistanceTo(Actor);
        if (CoverDistance >= ClosestCoverDistance) { continue; }

        // Assign as closest
        ClosestCoverDistance = CoverDistance;
        ClosestCover = *It;
    }
    return ClosestCover;
}

void ACoverSystem::GenerateGraph(float Radius)
{
    // Generate a random geometric graph of R depth between cover items
    for (UCoverNode* CurrentNode : CoverNodes)
    {
        for (UCoverNode* Node : CoverNodes)
        {
            // Check that they are within the radius and not itself
            if (Node->CoverActor->GetDistanceTo(CurrentNode->CoverActor) <= Radius && Node->CoverActor != CurrentNode->CoverActor)
            {
                CurrentNode->AdjacentCover.Add(Node);
            }
        }
    }
}

void ACoverSystem::DisplayDebugGraph(float DisplayTime)
{
    // Display debug lines
    for (UCoverNode* Node : CoverNodes)
    {
        for (UCoverNode* ConnectedNode : Node->AdjacentCover)
        {
            DrawDebugLine(GetWorld(), Node->CoverActor->GetActorLocation(),
                          ConnectedNode->CoverActor->GetActorLocation(), FColor::Green, false, DisplayTime);
        }
    }
}

void ACoverSystem::DisplayDebugPath(TArray<ACover*>* Path, float DisplayTime)
{
    // Display debug path
    for (auto It = Path->CreateConstIterator(); It+1; It++)
    {
        DrawDebugLine(GetWorld(), (*It)->GetActorLocation(),
                      (*(It+1))->GetActorLocation(), FColor::Red, false, DisplayTime);
    }
}