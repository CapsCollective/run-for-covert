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

    // Public overrides

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Public functions

    virtual void OnDeath();

    UFUNCTION(BlueprintPure)
    UHealthComponent* GetHealth() const;

    UFUNCTION(BlueprintNativeEvent)
    void BeginReload();

    // Components

    UPROPERTY(VisibleAnywhere)
    UHealthComponent* Health;

    // Public fields

    UPROPERTY(BlueprintReadWrite)
    bool bIsDead;

protected:

    // Protected overrides

	virtual void BeginPlay() override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent,
                             class AController* EventInstigator, AActor* DamageCauser) override;

    
    // Protected methods

    virtual bool Fire();

    UFUNCTION(BlueprintCallable)
    virtual void ReloadInitiated(float Length);

    void CancelReload();

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

};
