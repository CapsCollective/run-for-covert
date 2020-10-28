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

    virtual void OnDeath() override;

    virtual FVector GetAimVector() override;

    virtual void ApplyRecoil(FRotator& Recoil) override;

    // Properties

    UPROPERTY(EditAnywhere, Category = "Player Control")
    float MoveSpeed;

    UPROPERTY(EditAnywhere, Category = "Player Control")
    float LookSpeed;

    UPROPERTY(EditAnywhere, Category = "Player Control")
    float CrouchMultiplier;

    UPROPERTY(EditAnywhere, Category = "Player Control")
    float SprintMultiplier;

    UPROPERTY(EditAnywhere, Category = "Player Control")
    float CrouchHeight;
	
    // Components

    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USkeletalMeshComponent* Arms;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* MeshOffset;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionStimuliSourceComponent* AIStimulusSource;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UPawnNoiseEmitterComponent* PawnNoiseEmitter;
	
    // Input methods

    void MoveForward(float Amount);

    void MoveRight(float Amount);

    void LookUp(float Amount);

    void LookRight(float Amount);

    void FireStart();

    void FireEnd();

    void CrouchStart();

    void CrouchEnd();

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    float StandingHeight;

    float CrouchSpeed;

    // Private functions

    void PerformCrouch(bool bCrouch, float HalfHeight, float Offset);

    void ApplyCrouch(bool bCrouch, float HalfHeight, float Offset);

    UFUNCTION(Server, Reliable)
    void ServerApplyCrouch(bool bCrouch, float HalfHeight, float Offset);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastApplyCrouch(bool bCrouch, float HalfHeight, float Offset);

};
