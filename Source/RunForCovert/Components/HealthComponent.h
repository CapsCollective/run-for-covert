// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNFORCOVERT_API UHealthComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UHealthComponent();

    // Properties

    UPROPERTY(EditAnywhere, Category = "Health")
    float MaxHealth;

    // Public functions

    void OnTakeDamage(float Damage);

    UFUNCTION(BlueprintPure)
    float GetHealthPercentage() const;

protected:

    // Protected overrides

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

    // Private fields

    UPROPERTY(Replicated)
    float CurrentHealth;

    // Private functions

    void OnDeath();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastOnDeath();

};
