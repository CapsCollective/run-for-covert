// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "RunForCovert/Objects/State.h"
#include "PatrolState.generated.h"


UCLASS()
class RUNFORCOVERT_API UPatrolState : public UState
{
	GENERATED_BODY()

public:

    // Public overrides

	virtual void OnUpdate(AEnemyAIController& Owner) override;

    virtual UClass* ToTransition(AEnemyAIController& Owner) const override;

private:

    // Private fields

    UPROPERTY(VisibleAnywhere)
    class APatrol* PatrolPoint;
};
