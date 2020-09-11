// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../State.h"
#include "FireState.generated.h"


UCLASS()
class RUNFORCOVERT_API UFireState : public UState
{
	GENERATED_BODY()

public:

    UFireState();

    // Public overrides

    virtual void OnEnter(AEnemyAIController& Owner) override;

    virtual void OnExit(AEnemyAIController& Owner) override;

    virtual void OnUpdate(AEnemyAIController& Owner) override;

    virtual UClass* ToTransition(AEnemyAIController& Owner) const override;

private:

	// Private fields

    int32 TimesToFire;

    int32 TimesFired;

	float FireDelay;

	float TimeStarted;
	
};
