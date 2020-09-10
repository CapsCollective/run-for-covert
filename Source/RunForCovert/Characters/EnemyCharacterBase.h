// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.h"
#include "Perception/AIPerceptionComponent.h"

#include "EnemyCharacterBase.generated.h"

UCLASS()
class RUNFORCOVERT_API AEnemyCharacterBase : public ACharacterBase
{

	GENERATED_BODY()

public:

	AEnemyCharacterBase();

    // Public overrides

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* PerceptionComponent;
	
    // Public methods

    void FireWeapon();

	UFUNCTION()
	void SeePlayer(AActor* ActorSensed, FAIStimulus Stimulus);

	UPROPERTY()
	float TimeToSeePlayer;

	float SeenPlayerFor;
	bool bSeePlayer;
	bool bChasePlayer;

	UPROPERTY(BlueprintReadOnly)
	bool IsCrouching;
	
	void SetCrouching(bool Crouching);
	
protected:

    // Protected overrides

	virtual void BeginPlay() override;

};
