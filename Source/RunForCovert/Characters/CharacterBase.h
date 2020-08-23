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

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Components

    UPROPERTY(VisibleAnywhere)
    UHealthComponent* Health;

protected:

    // Protected overrides

	virtual void BeginPlay() override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent,
                             class AController* EventInstigator, AActor* DamageCauser) override;

    // Protected virtual methods

    virtual void Fire();

    // Protected getters

    AGunBase* GetGun();

private:

    // Private fields

    AGunBase* Gun;

};
