// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../Transition.h"
#include "TakenCoverTransition.generated.h"

/**
 * 
 */
UCLASS()
class RUNFORCOVERT_API UTakenCoverTransition : public UTransition
{
	GENERATED_BODY()

public:
    virtual bool ToTransition(AEnemyAIController& Owner) const override;
	
};
