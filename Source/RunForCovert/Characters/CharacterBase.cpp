// Caps Collective 2020


#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    bIsDead = false;
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

    // Enable character crouching
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::OnDeath()
{
    bIsDead = true;
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

bool ACharacterBase::Fire()
{
    if (!GetGun()) { return false; }
    return GetGun()->Fire(GetController(), GetActorForwardVector());
}