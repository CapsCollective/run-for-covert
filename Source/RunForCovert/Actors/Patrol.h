// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunForCovert/Objects/PatrolNode.h"
#include "Patrol.generated.h"

UCLASS()
class RUNFORCOVERT_API APatrol : public AActor
{
	GENERATED_BODY()
	
public:

    // Public fields

	UPROPERTY(VisibleAnywhere)
	UPatrolNode* Node;

	UPROPERTY(EditAnywhere)
	TArray<APatrol*> AdjacentNodes;

	// Public functions

    void AddNode(UPatrolNode* NewNode);
	
};
