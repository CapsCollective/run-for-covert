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

    // Public functions

    virtual void OnDeath();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastOnDeath();

    virtual FVector GetAimVector();

    virtual void ApplyRecoil(FRotator& Recoil);

    UFUNCTION(BlueprintNativeEvent)
    void SetGun(AGunBase* GunActor);

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

    // Pure functions

    UFUNCTION(BlueprintPure)
    UHealthComponent* GetHealth() const;

    UFUNCTION(BlueprintPure)
    bool IsDead() const;

    UFUNCTION(BlueprintPure)
    bool IsReloading() const;

    UFUNCTION(BlueprintPure)
    AGunBase* GetGun() const;

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

private:

    // Private functions

    void ReloadEnd();

    AGunBase* FindGun();

    // Private fields

    UPROPERTY()
    AGunBase* Gun;

    UPROPERTY()
    FTimerHandle ReloadTimer;

    UPROPERTY(Replicated)
    bool bIsDead;

    float WalkSpeed;

    float SprintSpeed;

};
