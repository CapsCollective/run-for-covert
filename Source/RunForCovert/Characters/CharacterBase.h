// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunForCovert/Components/HealthComponent.h"
#include "RunForCovert/Actors/GunBase.h"
#include "CharacterBase.generated.h"

UCLASS()
class RUNFORCOVERT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	ACharacterBase();

    // Public properties

    UPROPERTY(BlueprintReadOnly)
    bool bIsSprinting;
    
    // Public functions

    virtual void OnDeath();

    virtual FVector GetAimVector();

    virtual void ApplyRecoil(FRotator& Recoil);

    virtual void SetCrouching(bool bCrouch);

    UFUNCTION(BlueprintImplementableEvent)
    void OnGunSet();

    UFUNCTION(BlueprintImplementableEvent)
    void OnFired();

    UFUNCTION(BlueprintNativeEvent)
    void BeginReload(float Length = 1.f);

    void SprintStart();

    UFUNCTION(Server, Reliable)
    void ServerSprintStart();

    void SprintEnd();

    UFUNCTION(Server, Reliable)
    void ServerSprintEnd();

    void GunLookup();

    // Pure functions

    UFUNCTION(BlueprintPure)
    UHealthComponent* GetHealth() const;

    UFUNCTION(BlueprintPure)
    bool IsDead() const;

    UFUNCTION(BlueprintPure)
    bool IsReloading() const;

    UFUNCTION(BlueprintPure)
    AGunBase* GetGun() const;

    UFUNCTION(BlueprintPure)
    bool IsCrouching() const;

    // Components

    UPROPERTY(VisibleAnywhere)
    UHealthComponent* Health;

protected:

    // Protected overrides

	virtual void BeginPlay() override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator, AActor* DamageCauser) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Protected methods

    virtual void Fire();

    UFUNCTION(Server, Reliable)
    void ServerBeginReload(float Length = 1.f);

    void CancelReload();

    void ApplySprintMultiplier(float SprintMultiplier);

    UFUNCTION(Server, Reliable)
    void ServerCancelReload();

    float GetWalkSpeed() const;

private:

    // Private functions

    void ReloadEnd();

    // Private fields

    UPROPERTY()
    AGunBase* Gun;

    UPROPERTY()
    FTimerHandle ReloadTimer;

    UPROPERTY()
    FTimerHandle GunSearchHandle;

    UPROPERTY(Replicated)
    bool bIsDead;

    UPROPERTY(Replicated)
    bool bIsCrouching;

    float WalkSpeed;

    float SprintSpeed;

};
