// Caps Collective 2020


#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
    Gun = nullptr;
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

void ACharacterBase::OnDeath()
{
    bIsDead = true;

    // Detach the character from their controller and disable collision
    DetachFromControllerPendingDestroy();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent,
                                       class AController* EventInstigator, AActor* DamageCauser)
{
    DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (!Health) { return 0.f; }
    Health->OnTakeDamage(DamageAmount);
    return DamageAmount;
}

FVector ACharacterBase::GetAimVector()
{
    return GetActorForwardVector();
}

AGunBase* ACharacterBase::GetGun() const
{
    return Gun;
}

UHealthComponent* ACharacterBase::GetHealth() const
{
    return Health;
}

void ACharacterBase::Fire()
{
    if (!GetGun()) { return; }
    CancelReload();
    GetGun()->SetTriggerDown(true);
}

void ACharacterBase::BeginReload_Implementation()
{
    ReloadInitiated(1.f);
}

void ACharacterBase::ReloadInitiated(float Length)
{
    GetWorldTimerManager().SetTimer(ReloadTimer, this, &ACharacterBase::ReloadEnd, Length, false);
}

void ACharacterBase::ReloadEnd()
{
    if (!GetGun()) { return; }
    GetGun()->Reload();
}

void ACharacterBase::CancelReload()
{
    GetWorldTimerManager().ClearTimer(ReloadTimer);
}

bool ACharacterBase::IsReloading()
{
    return GetWorldTimerManager().IsTimerActive(ReloadTimer);
}