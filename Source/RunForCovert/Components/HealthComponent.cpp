// Caps Collective 2020


#include "HealthComponent.h"
#include "RunForCovert/Characters/CharacterBase.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    // Set field default values
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHealthComponent, CurrentHealth)
}

void UHealthComponent::OnTakeDamage(float Damage)
{
    if ((CurrentHealth -= Damage) <= 0.f) {
        CurrentHealth = 0;
        OnDeath();
    }
}

void UHealthComponent::OnDeath()
{
    ACharacterBase* Character = Cast<ACharacterBase>(GetOwner());
    if (!Character) { return; }

    // Inform the character of their death
    Character->OnDeath();
}

float UHealthComponent::GetHealthPercentage() const
{
    return CurrentHealth / MaxHealth;
}