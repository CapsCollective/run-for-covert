// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "RunForCovert/Controllers/EnemyAIController.h"
#include "UObject/NoExportTypes.h"
#include "Transition.generated.h"

/**
 * 
 */
UCLASS()
class RUNFORCOVERT_API UTransition : public UObject
{
	GENERATED_BODY()

	public:
	
	virtual bool ToTransition(AEnemyAIController* Owner) const;
};
