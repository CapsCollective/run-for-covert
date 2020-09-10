// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "../Transition.h"
#include "PlayerUnseenTransition.generated.h"

/**
 * 
 */
UCLASS()
class RUNFORCOVERT_API UPlayerUnseenTransition : public UTransition
{
	GENERATED_BODY()

public:

    // Public override

    virtual bool ToTransition(AEnemyAIController& Owner) const override;
	
};
