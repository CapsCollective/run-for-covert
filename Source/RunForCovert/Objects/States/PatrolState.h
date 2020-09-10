// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "RunForCovert/Objects/State.h"

#include "PatrolState.generated.h"

/**
 * 
 */
UCLASS()
class RUNFORCOVERT_API UPatrolState : public UState
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere)
	APatrol* PatrolPoint;
	
	virtual void OnEnter(AEnemyAIController* Owner) override;
	virtual void OnExit(AEnemyAIController* Owner) override;
	virtual void OnUpdate(AEnemyAIController* Owner) override;
};
