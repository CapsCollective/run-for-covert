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

    UHoldCoverState();

    // Public overrides

	virtual void OnEnter() override;

	virtual void OnExit() override;

	virtual void OnUpdate() override;

    virtual UClass* ToTransition() const override;

private:

    // Private fields

    float TimeToCover;

    float TimeStarted;

};
