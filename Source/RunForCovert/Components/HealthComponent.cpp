// Caps Collective 2020


#include "RunForCovert/Characters/CharacterBase.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

    Character->DetachFromControllerPendingDestroy();
    Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}