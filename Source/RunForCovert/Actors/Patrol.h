// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Patrol.generated.h"

UCLASS()
class RUNFORCOVERT_API APatrol : public AActor
{
	GENERATED_BODY()
	
public:

    // Public fields

	UPROPERTY(VisibleAnywhere)
	class UGraphNode* Node;

	UPROPERTY(EditAnywhere)
	TArray<APatrol*> AdjacentNodes;

	// Public functions

    void AddNode(UGraphNode* NewNode);
	
};
