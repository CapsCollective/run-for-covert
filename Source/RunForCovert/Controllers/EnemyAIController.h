// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class RUNFORCOVERT_API AEnemyAIController : public AAIController
{

	GENERATED_BODY()

public:

    AEnemyAIController();

    // Public overrides

    virtual void Tick(float DeltaTime) override;

    // Public functions

    void FireAtPlayer();

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    APawn* Player;

    class AEnemyCharacterBase* Character;

    FTimerHandle TimerHandle;

    int32 RepeatedAction;

};