#include "PatrolSystem.h"
#include "EngineUtils.h"
#include "RunForCovert/Actors/Patrol.h"
#include "DrawDebugHelpers.h"

UPatrolSystem::UPatrolSystem()
{

}

void UPatrolSystem::Initialise(UWorld* InWorld)
{
    // Set the world reference
    World = InWorld;

    // Populate a list of nodes representing all cover actors in the
    // world wrapped in objects specifying graph attributes
    for (APatrol* Cover : TActorRange<APatrol>(World))
    {
        // Add cover node list
        UPatrolNode* NewNode = NewObject<UPatrolNode>();
        NewNode->PatrolActor = Cover;
        PatrolNodes.Add(NewNode);
    }

    // Generate the graph connections
    //GenerateGraph(10000.0f);
    DisplayDebugGraph(2.f);

    for(UPatrolNode* Node : PatrolNodes)
    {
        Node->PatrolActor->AddNode(Node);
    }
}

APatrol* UPatrolSystem::FindClosestValidPatrolPoint(AActor* Agent)
{
    // Iterate through all cover points in world
    float ClosestCoverDistance = TNumericLimits<float>::Max();
    APatrol* ClosestPatrolPoint = nullptr;
    for (APatrol* Cover : TActorRange<APatrol>(World))
    {
        float CoverDistance = Cover->GetDistanceTo(Agent);
        if (CoverDistance <= ClosestCoverDistance)
        {
            ClosestCoverDistance = CoverDistance;
            ClosestPatrolPoint = Cover;
        }
    }
    
    return ClosestPatrolPoint;
}

TArray<APatrol*> UPatrolSystem::FindPath(AActor* Agent, AActor* Enemy)
{
    // Set up the search for values
    TArray<UPatrolNode*> OpenSet;
    UPatrolNode* StartNode = GetClosestPatrol(Agent);
    UPatrolNode* EndNode = GetClosestPatrol(Enemy, true, Agent);
    check(StartNode && EndNode);

    // Reset the GScore of all nodes in the collection
    for (auto It = PatrolNodes.CreateConstIterator(); It; It++)
    {
        (*It)->GScore = TNumericLimits<float>::Max();
    }

    // Setup start node and add to open set
    StartNode->GScore = 0.f;
    StartNode->HScore = StartNode->PatrolActor->GetDistanceTo(EndNode->PatrolActor);
    OpenSet.Add(StartNode);

    // Iterate through open set while the collection contains items
    while (OpenSet.Num() > 0)
    {
        // Get the node with the lowest FScore
        OpenSet.Sort([](UPatrolNode &A, UPatrolNode &B){ return A.FScore() < B.FScore(); });
        UPatrolNode* CurrentNode = OpenSet.Pop();

        // Once the end node is reached, generate and return a path
        if (CurrentNode == EndNode) {
            TArray<APatrol*> Path;
            Path.Push(EndNode->PatrolActor);
            CurrentNode = EndNode;
            while (CurrentNode != StartNode)
            {
                CurrentNode = CurrentNode->CameFrom;
                Path.Add(CurrentNode->PatrolActor);
            }
            DisplayDebugPath(&Path, .5f);
            return Path;
        }

        // Explore each cover node adjacent to current
        for (UPatrolNode* ConnectedNode : CurrentNode->AdjacentNodes)
        {
            // Calculate tentative node GScore and check if it's better than the node it comes from
            float TentativeGScore = CurrentNode->GScore + CurrentNode->PatrolActor->GetDistanceTo(ConnectedNode->PatrolActor);
            if (TentativeGScore < ConnectedNode->GScore)
            {
                // Set scores and add to open set
                ConnectedNode->CameFrom = CurrentNode;
                ConnectedNode->GScore = TentativeGScore;
                ConnectedNode->HScore = ConnectedNode->PatrolActor->GetDistanceTo(EndNode->PatrolActor);
                if (!OpenSet.Contains(ConnectedNode))
                {
                    OpenSet.Add(ConnectedNode);
                }
            }
        }
    }
    // Return empty if no valid path is found
    return TArray<APatrol*>();
}

UPatrolNode* UPatrolSystem::GetClosestPatrol(AActor* Actor, bool MustBeUnoccupied, AActor* OtherAgent)
{
    // Iterate through all cover points in world
    float ClosestCoverDistance = TNumericLimits<float>::Max();
    UPatrolNode* ClosestCover = nullptr;
    for (auto It = PatrolNodes.CreateConstIterator(); It; It++)
    {
        // Check if the current point is closer
        float CoverDistance = (*It)->PatrolActor->GetDistanceTo(Actor);
        if (CoverDistance >= ClosestCoverDistance) { continue; }

        // Check to make sure it is available if unoccupied flag is set
        // if (MustBeUnoccupied && (*It)->PatrolActor->IsOccupiedByOther(OtherAgent)) { continue; }

        // Assign as closest
        ClosestCoverDistance = CoverDistance;
        ClosestCover = *It;
    }
    return ClosestCover;
}

void UPatrolSystem::GenerateGraph(float Radius)
{
    // Generate a random geometric graph of R depth between cover items
    for (UPatrolNode* CurrentNode : PatrolNodes)
    {
        for (UPatrolNode* Node : PatrolNodes)
        {
            // Check that they are within the radius and not itself
            if (Node->PatrolActor->GetDistanceTo(CurrentNode->PatrolActor) <= Radius && Node->PatrolActor != CurrentNode->PatrolActor)
            {
                CurrentNode->AdjacentNodes.Add(Node);
            }
        }
    }
}

void UPatrolSystem::DisplayDebugGraph(float DisplayTime)
{
    // Display debug lines
    for (UPatrolNode* Node : PatrolNodes)
    {
        for (UPatrolNode* ConnectedNode : Node->AdjacentNodes)
        {
            DrawDebugLine(World, Node->PatrolActor->GetActorLocation(),
                          ConnectedNode->PatrolActor->GetActorLocation(), FColor::Green, false, DisplayTime);
        }
    }
}

void UPatrolSystem::DisplayDebugPath(TArray<APatrol*>* Path, float DisplayTime)
{
    // Display debug path
    for (auto It = Path->CreateConstIterator(); It+1; It++)
    {
        DrawDebugLine(World, (*It)->GetActorLocation(),
                      (*(It+1))->GetActorLocation(), FColor::Red, false, DisplayTime);
    }
}