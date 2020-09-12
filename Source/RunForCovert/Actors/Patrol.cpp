// Caps Collective 2020


#include "Patrol.h"
#include "EngineUtils.h"

void APatrol::AddNode(UPatrolNode* NewNode)
{
	Node = NewNode;

	for (auto& It : AdjacentNodes)
	{
		Node->AdjacentNodes.Add(It->Node);
	}
}

