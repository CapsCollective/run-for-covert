// Caps Collective 2020


#include "Patrol.h"

#include "EngineUtils.h"

// Sets default values
APatrol::APatrol()
{
	
}

// Called when the game starts or when spawned
void APatrol::BeginPlay()
{
	Super::BeginPlay();
}

void APatrol::AddNode(UPatrolNode* NewNode)
{
	Node = NewNode; 


	for (auto& It : AdjacentNodes)
	{
		Node->AdjacentNodes.Add(It->Node);
	}
}

