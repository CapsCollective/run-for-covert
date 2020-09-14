// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "RunForCovert/Controllers/EnemyAIController.h"
#include "RunForCovert/Characters/EnemyCharacterBase.h"
#include "State.generated.h"

UCLASS()
class RUNFORCOVERT_API UState : public UObject
{
	GENERATED_BODY()

public:

    // Public virtual functions

    virtual void Initialise(AEnemyAIController* Owner);

	virtual void OnEnter();

	virtual void OnExit();

	virtual void OnUpdate();

    virtual UClass* ToTransition() const;

protected:

    // Protected fields

    UPROPERTY()
    AEnemyAIController* Owner;

};
