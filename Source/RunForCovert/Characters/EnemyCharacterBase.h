// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyCharacterBase.generated.h"

class UAISenseConfig_Damage;
UCLASS()
class RUNFORCOVERT_API AEnemyCharacterBase : public ACharacterBase
{

	GENERATED_BODY()

public:

	AEnemyCharacterBase();

    // Public overrides

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
    // Public methods

    bool FireWeapon();

	UFUNCTION()
	void SeePlayer(AActor* ActorSensed, FAIStimulus Stimulus);

    void SetCrouching(bool Crouching);

    // Components

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY(EditAnywhere)
	UAISenseConfig_Sight* SightConfig;

    // Public fields

    UPROPERTY(EditAnywhere)
    float FiringRange;

	UPROPERTY(EditAnywhere)
	float TimeToSeePlayer;

	bool bSeenPlayer;

	bool bChasingPlayer;
	
protected:

    // Protected overrides

	virtual void BeginPlay() override;

private:

    // Private fields

    float SeenPlayerFor;

};
