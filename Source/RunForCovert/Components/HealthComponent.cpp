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
    // Deduct damage from current health and check if they are dead
    if ((CurrentHealth -= Damage) <= 0.f) {
        CurrentHealth = 0;
        OnDeath();
    }
}

void UHealthComponent::OnDeath()
{
    // Get the owning character
    ACharacterBase* Character = Cast<ACharacterBase>(GetOwner());
    if (!Character) { return; }

    // Inform the character of their death via multicast
    Character->MulticastOnDeath();
}

float UHealthComponent::GetHealthPercentage() const
{
    return CurrentHealth / MaxHealth;
}