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

    // Public overrides

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Properties

    UPROPERTY(EditAnywhere, Category = "Health")
    float MaxHealth;

    // Public functions

    void OnTakeDamage(float Damage);

protected:

    // Protected overrides

	virtual void BeginPlay() override;

private:

    // Private fields

    float CurrentHealth;

    // Private functions

    void OnDeath();

};
