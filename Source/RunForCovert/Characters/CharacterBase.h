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

    virtual FVector GetAimVector();

    virtual void ApplyRecoil(FRotator& Recoil);

    UFUNCTION(BlueprintPure)
    UHealthComponent* GetHealth() const;

    UFUNCTION(BlueprintNativeEvent)
    void BeginReload();

    UFUNCTION(BlueprintImplementableEvent)
    void OnFired();

    UFUNCTION(BlueprintPure)
    bool IsDead() const;

    // Components

    UPROPERTY(VisibleAnywhere)
    UHealthComponent* Health;

protected:

    // Protected overrides

	virtual void BeginPlay() override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent,
                             class AController* EventInstigator, AActor* DamageCauser) override;

    // Protected methods

    virtual void Fire();

    UFUNCTION(BlueprintCallable)
    virtual void ReloadInitiated(float Length);

    void CancelReload();

    UFUNCTION(BlueprintPure)
    bool IsReloading() const;

    // Protected getters

    UFUNCTION(BlueprintPure)
    AGunBase* GetGun() const;

private:

    // Private functions

    void ReloadEnd();

    // Private fields

    UPROPERTY()
    AGunBase* Gun;

    UPROPERTY()
    FTimerHandle ReloadTimer;

    bool bIsDead;

};
