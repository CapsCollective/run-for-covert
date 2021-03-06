// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyCharacterBase.generated.h"

class UAISenseConfig_Damage;
UCLASS()
class RUNFORCOVERT_API AEnemyCharacterBase : public ACharacterBase
{

	GENERATED_BODY()

public:

	AEnemyCharacterBase();
	
    // Public methods

    virtual void Fire() override;

    virtual void SetCrouching(bool Crouching) override;

    // Public fields

    UPROPERTY(EditAnywhere)
    float FiringRange;

	UPROPERTY(EditAnywhere)
	float TimeToSeePlayer;

    UPROPERTY(EditAnywhere)
    float TurningSpeed;

};
