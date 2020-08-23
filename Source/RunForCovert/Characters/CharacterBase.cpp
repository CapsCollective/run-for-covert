// Caps Collective 2020


#include "CharacterBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
    TArray<AActor*> ChildActors;

    // Set up gun child actor
    GetAllChildActors(OUT ChildActors);
    if (ChildActors.Num() != 1)
    {
        UE_LOG(LogTemp, Error, TEXT("Incorrect number of child actors for character."))
        return;
    }
    Gun = Cast<AGunBase>(ChildActors[0]);
    if (!Gun)
    {
        UE_LOG(LogTemp, Error, TEXT("Child actor does not inherit from AGunBase."))
        return;
    }
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent,
                                       class AController* EventInstigator, AActor* DamageCauser)
{
    DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (!Health) { return 0.f; }
    Health->OnTakeDamage(DamageAmount);
    return DamageAmount;
}

AGunBase* ACharacterBase::GetGun()
{
    return Gun;
}

void ACharacterBase::Fire()
{
    if (!GetGun()) { return; }
    GetGun()->Fire(GetController(), GetActorForwardVector());
}