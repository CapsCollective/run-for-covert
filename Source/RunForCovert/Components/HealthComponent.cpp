// Caps Collective 2020


#include "HealthComponent.h"
#include "RunForCovert/Characters/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    // Set field default values
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
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
    UE_LOG(LogTemp, Warning, TEXT("%s died!"), *GetOwner()->GetName())

    ACharacterBase* Character = Cast<ACharacterBase>(GetOwner());

    if (!Character) { return; }

    Character->OnDeath();
    Character->DetachFromControllerPendingDestroy();
    Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}