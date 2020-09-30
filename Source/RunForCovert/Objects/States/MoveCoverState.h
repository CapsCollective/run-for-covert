// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../State.h"
#include "MoveCoverState.generated.h"


UCLASS()
class RUNFORCOVERT_API UMoveCoverState : public UState
{
	GENERATED_BODY()

public:

    UMoveCoverState();

    // Public overrides

    virtual void OnEnter() override;

    virtual void OnExit() override;

    virtual void OnUpdate() override;

    virtual UClass* ToTransition() const override;

private:

    // Private fields

    bool bTakenFinalCover;

    UPROPERTY()
    class UCoverPointComponent* CoverPoint;

    UPROPERTY()
    TArray<class ACover*> CoverPath;

    UPROPERTY()
    UCoverPointComponent* PreviousCoverPoint;

};
