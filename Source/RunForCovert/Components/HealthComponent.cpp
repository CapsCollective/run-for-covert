// Caps Collective 2020


#include "HealthComponent.h"
#include "GameFramework/Actor.h"

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
    CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, CurrentHealth);
}

void UHealthComponent::OnDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("%s died!"), *GetOwner()->GetName())
}