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

    // Public overrides

    virtual void OnUpdate(AEnemyAIController& Owner) override;

private:

    // Private fields

    UPROPERTY()
    class UCoverPointComponent* CoverPoint;

    UPROPERTY()
    TArray<class ACover*> CoverPath;

    UPROPERTY()
    UCoverPointComponent* PreviousCoverPoint;

};
