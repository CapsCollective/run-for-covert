// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "RunForCovert/Characters/EnemyCharacterBase.h"
#include "RunForCovert/Controllers/EnemyAIController.h"
#include "UObject/NoExportTypes.h"
#include "State.generated.h"

/**
 * 
 */
UCLASS()
class RUNFORCOVERT_API UState : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnEnter();
	virtual void OnExit();
	virtual void OnUpdate(AEnemyAIController* Owner);
};
