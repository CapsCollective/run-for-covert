// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../State.h"
#include "ChargeState.generated.h"


UCLASS()
class RUNFORCOVERT_API UChargeState : public UState
{
	GENERATED_BODY()

public:

    UChargeState();

    // Public overrides

    virtual void OnEnter() override;

    virtual void OnExit() override;

    virtual void OnUpdate() override;

    virtual UClass* ToTransition() const override;

private:

    // Private fields

    bool bCharged;

};
