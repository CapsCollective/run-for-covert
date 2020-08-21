// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"

UCLASS()
class RUNFORCOVERT_API AEnemyCharacterBase : public ACharacter
{

	GENERATED_BODY()

public:

	AEnemyCharacterBase();

    // Public overrides

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

    // Protected overrides

	virtual void BeginPlay() override;

};
