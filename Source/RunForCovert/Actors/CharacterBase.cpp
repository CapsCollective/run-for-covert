// Caps Collective 2020


#include "CharacterBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
                                       class AController * EventInstigator, AActor * DamageCauser)
{
    if (!Health) { return 0.f; }
    Health->OnTakeDamage(DamageAmount);
    return DamageAmount;
}