// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PatrolNode.generated.h"


UCLASS()
class RUNFORCOVERT_API UPatrolNode : public UObject
{

	GENERATED_BODY()

	public:

	// Public fields

	UPROPERTY()
	class APatrol* PatrolActor;

	UPROPERTY()
	TArray<UPatrolNode*> AdjacentNodes;
	
	UPROPERTY()
	UPatrolNode* CameFrom;

	float GScore;

	float HScore;

	// Public functions

	float FScore();
	
};
