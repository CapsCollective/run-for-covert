// Caps Collective 2020


#include "Patrol.h"

#include "EngineUtils.h"

// Sets default values
APatrol::APatrol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Populate the cover points field
	// GetComponents(OUT CoverPoints);
	//
	// if (CoverPoints.Num() <= 0)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("No cover points found for %s"), *GetName())
	// }
	
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

// Called every frame
void APatrol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

