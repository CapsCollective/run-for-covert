// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../State.h"
#include "HoldCoverState.generated.h"


UCLASS()
class RUNFORCOVERT_API UHoldCoverState : public UState
{
	GENERATED_BODY()

public:

	virtual void OnEnter(AEnemyAIController& Owner) override;

	virtual void OnExit(AEnemyAIController& Owner) override;

	virtual void OnUpdate(AEnemyAIController& Owner) override;

private:

    // Private fields

    float TimeToCover = 2.f;

    float TimeStarted = 0.f;

};
