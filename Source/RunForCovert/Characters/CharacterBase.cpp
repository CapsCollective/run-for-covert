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

    // Setup components
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
    Health->SetIsReplicated(true);

    // Set field default values
    Gun = nullptr;
    bIsDead = false;
    bIsCrouching = false;
    SprintSpeed = 0.f;
    WalkSpeed = 0.f;
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACharacterBase, bIsCrouching)
    DOREPLIFETIME(ACharacterBase, bIsDead)
}

void ACharacterBase::BeginPlay()
{
    Super::BeginPlay();

    // Enable character crouching
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

    // Calculate movement speeds
    WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
    ApplySprintMultiplier(1.f);

    // Begin searching for the child gun
    GetWorldTimerManager().SetTimer(GunSearchHandle, this, &ACharacterBase::GunLookup, .5f, true);
}

void ACharacterBase::GunLookup()
{
    // Receive a list of all child actors and set the field if found
    TArray<AActor*> AttachedActors;
    GetAttachedActors(OUT AttachedActors);
    for (auto It = AttachedActors.CreateConstIterator(); It; It++)
    {
        if (AGunBase* GunActor = Cast<AGunBase>(*It))
        {
            Gun = GunActor;
            break;
        }
    }

    // Clear the timer and post to the event once found
    if (Gun)
    {
        GetWorldTimerManager().ClearTimer(GunSearchHandle);
        OnGunSet();
    }

    UE_LOG(LogTemp, Warning, TEXT("Searching for owning character..."))
}

void ACharacterBase::OnDeath()
{
    bIsDead = true;

    // Detach the character from their controller and disable collision
    DetachFromControllerPendingDestroy();
    GetCharacterMovement()->Deactivate();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ACharacterBase::TakeDamage(float DamageAmount,
                                 struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void ACharacterBase::ApplySprintMultiplier(float SprintMultiplier)
{
    SprintSpeed = WalkSpeed * SprintMultiplier;
}

void ACharacterBase::SprintStart()
{
    if (bIsCrouching) { return; }

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
    if (bIsCrouching) { return; }

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

float ACharacterBase::GetWalkSpeed() const
{
    return WalkSpeed;
}

void ACharacterBase::SetCrouching(bool bCrouch)
{
    bIsCrouching = bCrouch;
}

bool ACharacterBase::IsReloading() const
{
    return GetWorldTimerManager().IsTimerActive(ReloadTimer);
}

bool ACharacterBase::IsDead() const
{
    return bIsDead;
}

bool ACharacterBase::IsCrouching() const
{
    return bIsCrouching;
}

AGunBase* ACharacterBase::GetGun() const
{
    return Gun;
}

UHealthComponent* ACharacterBase::GetHealth() const
{
    return Health;
}