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

void ACharacterBase::ApplyRecoil(FRotator& Recoil)
{
    // Void virtual method implementation
}

void ACharacterBase::SetGun(AGunBase* GunActor)
{
    Gun = GunActor;
    OnGunSet();
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
    if (!Gun) { return; }
    CancelReload();
    Gun->SetTriggerDown(true);
}

void ACharacterBase::BeginReload_Implementation(float Length)
{
    if (IsReloading() || !Gun || Gun->FullyLoaded()) { return; }
    GetWorldTimerManager().SetTimer(ReloadTimer, this, &ACharacterBase::ReloadEnd, Length, false);
}

void ACharacterBase::ReloadEnd()
{
    if (!Gun) { return; }
    Gun->Reload();
}

void ACharacterBase::CancelReload()
{
    GetWorldTimerManager().ClearTimer(ReloadTimer);
}

bool ACharacterBase::IsReloading() const
{
    return GetWorldTimerManager().IsTimerActive(ReloadTimer);
}

bool  ACharacterBase::IsDead() const
{
    return bIsDead;
}