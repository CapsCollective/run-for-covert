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

    virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditAnywhere)
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

protected:

	// Protected overrides

	virtual void BeginPlay() override;

    virtual void Fire() override;

private:

    // Private fields

    float DefaultCapsuleHeight;
};
