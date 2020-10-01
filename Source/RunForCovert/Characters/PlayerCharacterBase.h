// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PlayerCharacterBase.generated.h"

UCLASS()
class RUNFORCOVERT_API APlayerCharacterBase : public ACharacterBase
{

	GENERATED_BODY()

public:

	APlayerCharacterBase();

	// Public overrides

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Properties

    UPROPERTY(EditAnywhere, Category = "Player Control")
    float MoveSpeed;

    UPROPERTY(EditAnywhere, Category = "Player Control")
    float LookSpeed;

    UPROPERTY(EditAnywhere, Category = "Player Control")
    float SprintMultiplier;
	
    // Components

    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USkeletalMeshComponent* Arms;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionStimuliSourceComponent* AIStimulusSource;
	
    // Input methods

    void MoveForward(float Amount);

    void MoveRight(float Amount);

    void LookUp(float Amount);

    void LookRight(float Amount);

    void SprintStart();

    void SprintEnd();

    void CrouchStart();

    void CrouchEnd();

    void FireWeapon();

protected:

	// Protected overrides

    virtual bool Fire() override;

    UFUNCTION(BlueprintImplementableEvent)
    void OnFired();

};
