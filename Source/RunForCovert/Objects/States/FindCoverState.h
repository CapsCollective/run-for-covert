// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../State.h"
#include "FindCoverState.generated.h"

/**
 * 
 */
UCLASS()
class RUNFORCOVERT_API UFindCoverState : public UState
{
	GENERATED_BODY()

public:
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void OnUpdate(AEnemyAIController* Owner) override;

private:

	UPROPERTY()
	TArray<class ACover*> CoverPath;
	
	UPROPERTY()
	class UCoverPointComponent* CoverPoint;

	UPROPERTY()
	UCoverPointComponent* PreviousCoverPoint;
};
