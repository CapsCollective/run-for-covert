// Caps Collective 2020


#include "Patrol.h"
#include "EngineUtils.h"
#include "../Objects/GraphNode.h"

void APatrol::AddNode(UGraphNode* NewNode)
{
	Node = NewNode;

	for (auto& It : AdjacentNodes)
	{
		Node->AdjacentNodes.Add(It->Node);
	}
}

