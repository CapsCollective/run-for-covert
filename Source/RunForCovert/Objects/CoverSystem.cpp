// Caps Collective 2020


#include "CoverSystem.h"
#include "EngineUtils.h"
#include "GraphNode.h"
#include "RunForCovert/Actors/Cover.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

UCoverSystem::UCoverSystem()
{
    // Set field default values
    CoverRadius = 1500.f;
    GenerationHeightOffset = FVector(0.f, 0.f, 1.f);
    World = nullptr;
}

void UCoverSystem::Initialise(UWorld* InWorld)
{
    // Set the world reference
    World = InWorld;

    // Populate a list of nodes representing all cover actors in the
    // world wrapped in objects specifying graph attributes
    for (ACover* Cover : TActorRange<ACover>(World))
    {
        // Add cover node list
        UGraphNode* NewNode = NewObject<UGraphNode>();
        NewNode->Actor = Cover;
        GraphNodes.Add(NewNode);
    }

    // Generate the graph connections
    GenerateGraph(CoverRadius);
    DisplayDebugGraph(2.f);
}

UCoverPointComponent* UCoverSystem::FindClosestValidCoverPoint(AActor* Agent, AActor* Enemy)
{
    // Iterate through all cover points in world
    float ClosestCoverDistance = TNumericLimits<float>::Max();
    UCoverPointComponent* ClosestCoverPoint = nullptr;
    for (auto It = GraphNodes.CreateConstIterator(); It; It++)
    {
        // Check if the current point is closer than stored
        float CoverDistance = (*It)->Actor->GetDistanceTo(Agent);
        if (CoverDistance >= ClosestCoverDistance) { continue; }

        // Check if it actually provides cover from the enemy
        UCoverPointComponent* PotentialCover = (*It)->GetActor<ACover>()->FindValidCoverPoint(Enemy->GetActorLocation());
        if (PotentialCover)
        {
            ClosestCoverDistance = CoverDistance;
            ClosestCoverPoint = PotentialCover;
        }
    }
    return ClosestCoverPoint;
}

TArray<ACover*> UCoverSystem::FindCoverPath(AActor* Agent, AActor* Enemy)
{
    // Set up the search for values
    TArray<UGraphNode*> OpenSet;
    UGraphNode* StartNode = GetClosestCover(Agent);
    UGraphNode* EndNode = GetClosestCover(Enemy, true, Agent);
    check(StartNode && EndNode)

    // Reset the GScore of all nodes in the collection
    for (auto It = GraphNodes.CreateConstIterator(); It; It++)
    {
        (*It)->GScore = TNumericLimits<float>::Max();
    }

    // Setup start node and add to open set
    StartNode->GScore = 0.f;
    StartNode->HScore = StartNode->Actor->GetDistanceTo(EndNode->Actor);
    OpenSet.Add(StartNode);

    // Iterate through open set while the collection contains items
    while (OpenSet.Num() > 0)
    {
        // Get the node with the lowest FScore
        OpenSet.Sort([](UGraphNode &A, UGraphNode &B){ return A.FScore() < B.FScore(); });
        UGraphNode* CurrentNode = OpenSet.Pop();

        // Once the end node is reached, generate and return a path
        if (CurrentNode == EndNode) {
            TArray<ACover*> Path;
            Path.Push(EndNode->GetActor<ACover>());
            CurrentNode = EndNode;
            while (CurrentNode != StartNode)
            {
                CurrentNode = CurrentNode->CameFrom;
                Path.Add(CurrentNode->GetActor<ACover>());
            }
            DisplayDebugPath(&Path, .5f);
            return Path;
        }

        // Explore each cover node adjacent to current
        for (UGraphNode* ConnectedNode : CurrentNode->AdjacentNodes)
        {
            // Calculate tentative node GScore and check if it's better than the node it comes from
            float TentativeGScore = CurrentNode->GScore + CurrentNode->Actor->GetDistanceTo(ConnectedNode->Actor);
            if (TentativeGScore < ConnectedNode->GScore)
            {
                // Set scores and add to open set
                ConnectedNode->CameFrom = CurrentNode;
                ConnectedNode->GScore = TentativeGScore;
                ConnectedNode->HScore = ConnectedNode->Actor->GetDistanceTo(EndNode->Actor);
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

UGraphNode* UCoverSystem::GetClosestCover(AActor* Actor, bool MustBeUnoccupied, AActor* OtherAgent)
{
    // Iterate through all cover points in world
    float ClosestCoverDistance = TNumericLimits<float>::Max();
    UGraphNode* ClosestCover = nullptr;
    for (auto It = GraphNodes.CreateConstIterator(); It; It++)
    {
        // Check if the current point is closer
        float CoverDistance = (*It)->Actor->GetDistanceTo(Actor);
        if (CoverDistance >= ClosestCoverDistance) { continue; }

        // Check to make sure it is available if unoccupied flag is set
        if (MustBeUnoccupied && (*It)->GetActor<ACover>()->IsOccupiedByOther(OtherAgent)) { continue; }

        // Assign as closest
        ClosestCoverDistance = CoverDistance;
        ClosestCover = *It;
    }
    return ClosestCover;
}

void UCoverSystem::GenerateGraph(float Radius)
{
    // Generate a random geometric graph of R depth between cover items
    for (UGraphNode* CurrentNode : GraphNodes)
    {
        for (UGraphNode* Node : GraphNodes)
        {
            // Check that they are within the radius and not itself
            if (Node->Actor->GetDistanceTo(CurrentNode->Actor) > Radius || Node->Actor == CurrentNode->Actor) { continue; }

            // Create default trace parameters and ignore the currently checked cover
            FHitResult Hit;
            FCollisionQueryParams QueryParams;
            QueryParams.AddIgnoredActor(CurrentNode->Actor);
            QueryParams.AddIgnoredActor(Node->Actor);

            // Perform a line trace between the cover actors
            bool bHitAnything = World->LineTraceSingleByChannel(
                    OUT Hit,
                    CurrentNode->Actor->GetActorLocation() + GenerationHeightOffset,
                    Node->Actor->GetActorLocation() + GenerationHeightOffset,
                    ECC_WorldDynamic,
                    QueryParams);

            // Link the cover nodes if the trace is clear
            if (!bHitAnything)
            {
                CurrentNode->AdjacentNodes.Add(Node);
            }
        }
    }
}

void UCoverSystem::DisplayDebugGraph(float DisplayTime)
{
    // Display debug lines
    for (UGraphNode* Node : GraphNodes)
    {
        for (UGraphNode* ConnectedNode : Node->AdjacentNodes)
        {
            DrawDebugLine(World, Node->Actor->GetActorLocation(),
                          ConnectedNode->Actor->GetActorLocation(), FColor::Green, false, DisplayTime);
        }
    }
}

void UCoverSystem::DisplayDebugPath(TArray<ACover*>* Path, float DisplayTime)
{
    // Display debug path
    for (auto It = Path->CreateConstIterator(); It+1; It++)
    {
        DrawDebugLine(World, (*It)->GetActorLocation(),
                      (*(It+1))->GetActorLocation(), FColor::Red, false, DisplayTime);
    }
}