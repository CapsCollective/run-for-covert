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

    virtual void OnEnter() override;

    virtual void OnExit() override;

    virtual void OnUpdate() override;

    virtual UClass* ToTransition() const override;

private:

	// Private fields

    int32 TimesToFire;

    int32 TimesFired;

	float FireDelay;

	float TimeStarted;
	
};
