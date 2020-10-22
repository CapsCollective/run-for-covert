// Caps Collective 2020


#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

    // Set actor for replication
    bReplicates = true;

    // Set field default values
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
    Gun = nullptr;
    bIsDead = false;
    SprintSpeed = 0.f;
    WalkSpeed = 0.f;
}

void ACharacterBase::BeginPlay()
{
    Super::BeginPlay();

    // Enable character crouching
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

    // Calculate movement speeds
    WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
    ApplySprintMultiplier(1.f);

    // Set the gun field
    SetGun(FindGun());
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
    // Empty virtual method implementation
}

void ACharacterBase::Fire()
{
    if (!Gun) { return; }
    CancelReload();
    Gun->SetTriggerDown(true);
}

void ACharacterBase::BeginReload_Implementation(float Length)
{
    // Start the same process on the server
    if (!HasAuthority())
    {
        ServerBeginReload(Length);
    }

    // Start a timer that reloads the gun on finishing
    if (IsReloading() || !Gun || Gun->FullyLoaded()) { return; }
    GetWorldTimerManager().SetTimer(ReloadTimer, Gun, &AGunBase::Reload, Length, false);
}

void ACharacterBase::ServerBeginReload_Implementation(float Length)
{
    // Start the reload process on the server
    BeginReload(Length);
}

void ACharacterBase::CancelReload()
{
    // Start the same process on the server
    if (!HasAuthority())
    {
        ServerCancelReload();
    }

    // Clear the reload timer
    GetWorldTimerManager().ClearTimer(ReloadTimer);
}

void ACharacterBase::ServerCancelReload_Implementation()
{
    // Cancel the reload process on the server
    CancelReload();
}

AGunBase* ACharacterBase::FindGun()
{
    // Receive a list of all child actors
    TArray<AActor*> AttachedActors;
    GetAttachedActors(OUT AttachedActors);

    // Return the gun child actor if found
    for (auto It = AttachedActors.CreateConstIterator(); It; It++)
    {
        if (AGunBase* GunActor = Cast<AGunBase>(*It))
        {
            return GunActor;
        }
    }
    return nullptr;
}

void ACharacterBase::SetGun(AGunBase* GunActor)
{
    if (Gun) { return; }
    Gun = GunActor;
}

void ACharacterBase::ApplySprintMultiplier(float SprintMultiplier)
{
    SprintSpeed = WalkSpeed * SprintMultiplier;
}

void ACharacterBase::SprintStart()
{
    // Start the same process on the server
    if (!HasAuthority())
    {
        ServerSprintStart();
    }

    // Apply the sprint speed
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ACharacterBase::ServerSprintStart_Implementation()
{
    SprintStart();
}

void ACharacterBase::SprintEnd()
{
    // Start the same process on the server
    if (!HasAuthority())
    {
        ServerSprintEnd();
    }

    // Reapply the walk speed
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ACharacterBase::ServerSprintEnd_Implementation()
{
    SprintEnd();
}

bool ACharacterBase::IsReloading() const
{
    return GetWorldTimerManager().IsTimerActive(ReloadTimer);
}

bool  ACharacterBase::IsDead() const
{
    return bIsDead;
}

AGunBase* ACharacterBase::GetGun() const
{
    return Gun;
}

UHealthComponent* ACharacterBase::GetHealth() const
{
    return Health;
}