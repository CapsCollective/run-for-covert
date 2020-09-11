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

    void OnDeath();

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

    
    // Protected virtual methods

    virtual bool Fire();

    // Protected getters

    AGunBase* GetGun();

private:

    // Private fields

    UPROPERTY()
    AGunBase* Gun;

};
