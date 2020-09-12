// Caps Collective 2020

#include "PatrolSystem.h"
#include "EngineUtils.h"
#include "GraphNode.h"
#include "RunForCovert/Actors/Patrol.h"
#include "DrawDebugHelpers.h"


void UPatrolSystem::Initialise(UWorld* InWorld)
{
    // Set the world reference
    World = InWorld;

    // Populate a list of nodes representing all patrol actors in the
    // world wrapped in objects specifying graph attributes
    for (APatrol* Patrol : TActorRange<APatrol>(World))
    {
        // Add patrol node list
        UGraphNode* NewNode = NewObject<UGraphNode>();
        NewNode->Actor = Patrol;
        GraphNodes.Add(NewNode);
    }

    DisplayDebugGraph(2.f);

    for(UGraphNode* Node : GraphNodes)
    {
        Node->GetActor<APatrol>()->AddNode(Node);
    }
}

// Finds the closest patrol point
APatrol* UPatrolSystem::FindClosestPatrolPoint(AActor* Agent)
{
    // Iterate through all patrol points in world
    float ClosestPatrolDistance = TNumericLimits<float>::Max();
    APatrol* ClosestPatrolPoint = nullptr;
    for (APatrol* Patrol : TActorRange<APatrol>(World))
    {
        float PatrolDistance = Patrol->GetDistanceTo(Agent);
        if (PatrolDistance <= ClosestPatrolDistance)
        {
            ClosestPatrolDistance = PatrolDistance;
            ClosestPatrolPoint = Patrol;
        }
    }
    
    return ClosestPatrolPoint;
}

// Returns the path that the AI should take.
TArray<APatrol*> UPatrolSystem::FindPath(AActor* Agent, AActor* Enemy)
{
    // Set up the search for values
    TArray<UGraphNode*> OpenSet;
    UGraphNode* StartNode = GetClosestPatrol(Agent);
    UGraphNode* EndNode = GetClosestPatrol(Enemy, true, Agent);
    check(StartNode && EndNode);

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
            TArray<APatrol*> Path;
            Path.Push(EndNode->GetActor<APatrol>());
            CurrentNode = EndNode;
            while (CurrentNode != StartNode)
            {
                CurrentNode = CurrentNode->CameFrom;
                Path.Add(CurrentNode->GetActor<APatrol>());
            }
            DisplayDebugPath(&Path, .5f);
            return Path;
        }

        // Explore each patrol node adjacent to current
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
    return TArray<APatrol*>();
}

// Returns the closest patrol point to the AI
UGraphNode* UPatrolSystem::GetClosestPatrol(AActor* Actor, bool MustBeUnoccupied, AActor* OtherAgent)
{
    // Iterate through all patrol points in world
    float ClosestPatrolDistance = TNumericLimits<float>::Max();
    UGraphNode* ClosestPatrol = nullptr;
    for (auto It = GraphNodes.CreateConstIterator(); It; It++)
    {
        // Check if the current point is closer
        float PatrolDistance = (*It)->Actor->GetDistanceTo(Actor);
        if (PatrolDistance >= ClosestPatrolDistance) { continue; }

        // Assign as closest
        ClosestPatrolDistance = PatrolDistance;
        ClosestPatrol = *It;
    }
    return ClosestPatrol;
}

void UPatrolSystem::DisplayDebugGraph(float DisplayTime)
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

void UPatrolSystem::DisplayDebugPath(TArray<APatrol*>* Path, float DisplayTime)
{
    // Display debug path
    for (auto It = Path->CreateConstIterator(); It+1; It++)
    {
        DrawDebugLine(World, (*It)->GetActorLocation(),
                      (*(It+1))->GetActorLocation(), FColor::Red, false, DisplayTime);
    }
}