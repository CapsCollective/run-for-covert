// Caps Collective 2020


#include "GunBase.h"
#include "../Characters/CharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "TimerManager.h"


AGunBase::AGunBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set actor for replication
    bReplicates = true;

	// Setup components
    GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
    RootComponent = GunMesh;

    MuzzlePosition = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Position"));
    MuzzlePosition->SetupAttachment(GunMesh);

    // Set field default values
    FireSound = nullptr;
    ClipEmptySound = nullptr;
    RicochetSound = nullptr;
    MuzzleFlashEffect = nullptr;
    HitCharacterEffect = nullptr;
    HitSurfaceEffect = nullptr;
    OwningCharacter = nullptr;
    bCanFire = true;
    bAutomatic = false;
    bTriggerDown = false;
    GunDamage = 10.f;
    MaxFireRate = .5f;
    LastFireTime = 0.f;
    BulletSpeed = 10000.f;
    BulletSpread = 0.f;
    Recoil = 1.f;
    MagazineSize = 16;
    CurrentAmmo = 0;
}

void AGunBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AGunBase, CurrentAmmo)
    DOREPLIFETIME(AGunBase, bCanFire)
}

void AGunBase::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        // Set the magazine to full
        CurrentAmmo = MagazineSize;
    }

    // Begin searching for the owning character
    GetWorldTimerManager().SetTimer(CharacterSearchHandle, this, &AGunBase::OwningCharacterLookup, .5f, true);
}

void AGunBase::OwningCharacterLookup()
{
    // Try find the owning character for setup
    OwningCharacter = Cast<ACharacterBase>(GetParentActor());
    if (OwningCharacter)
    {
        // Perform setup
        SetOwner(OwningCharacter);

        // Clear the timer once found
        GetWorldTimerManager().ClearTimer(CharacterSearchHandle);
    }

    UE_LOG(LogTemp, Warning, TEXT("Searching for owning character..."))
}

void AGunBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Allow the server to clear the client for firing based on ammo and fire time
    if (HasAuthority())
    {
        bCanFire = (LastFireTime + MaxFireRate <= GetWorld()->GetGameState()->GetServerWorldTimeSeconds()) & (CurrentAmmo > 0);
    }

    if (bTriggerDown && bCanFire && OwningCharacter)
    {
        // Calculate launch velocity
        FVector LaunchVelocity = FMath::VRand() * BulletSpread + OwningCharacter->GetAimVector() * BulletSpeed;

        // Run fire on the local machine and then forward to relevant partner type
        Fire(LaunchVelocity);
        HasAuthority() ? MulticastFire(LaunchVelocity) : ServerFire(LaunchVelocity);

        // Stop firing until the server updates it again
        bCanFire = false;

        // Reset the trigger for non-automatic weapons
        if (!bAutomatic && HasAuthority()) { bTriggerDown = false; }
    }
}

void AGunBase::SetTriggerDown(bool bPulled)
{
    // Play the clip empty sound if there is no ammo
    if (bPulled && CurrentAmmo <= 0)
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ClipEmptySound,
                                               MuzzlePosition->GetComponentLocation());
    }
    bTriggerDown = bPulled;
}

void AGunBase::Fire(FVector LaunchVelocity)
{
    if (!OwningCharacter || !GetWorld()->GetGameState()) { return; }

    // Decrement ammunition and set the last fire time
    if (HasAuthority())
    {
        --CurrentAmmo;
        LastFireTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
    }

    // Play firing sound and particle effect
    UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, GunMesh, TEXT("Muzzle"));
    UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound,
                                           MuzzlePosition->GetComponentLocation());

    // Get projectile trace
    FPredictProjectilePathResult Result;
    FPredictProjectilePathParams Params = FPredictProjectilePathParams(
            1.f,
            MuzzlePosition->GetComponentLocation(),
            LaunchVelocity,
            2.f,
            ECC_WorldDynamic,
            GetParentActor()
    );
    if (UGameplayStatics::PredictProjectilePath(GetWorld(), Params, OUT Result))
    {
        // Spawn an impact effect at the impact point
        ACharacter* HitCharacter =  Cast<ACharacter>(Result.HitResult.GetActor());
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
                               HitCharacter ? RicochetSound : nullptr, Result.HitResult.ImpactPoint);
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
                                 HitCharacter ? HitCharacterEffect : HitSurfaceEffect,
                                 Result.HitResult.ImpactPoint, LaunchVelocity.Rotation().GetInverse());

        // Damage actor
        if (HasAuthority() && OwningCharacter->GetController())
        {
            if (!Result.HitResult.GetActor())
            { return; }
            FPointDamageEvent DamageEvent = FPointDamageEvent(GunDamage, Result.HitResult,
                                                              Result.HitResult.ImpactNormal, nullptr);
            Result.HitResult.GetActor()->TakeDamage(GunDamage, DamageEvent,
                                                    OwningCharacter->GetController(), GetParentActor());
        }
    }

    // Apply recoil to the character
    if (HasAuthority())
    {
        FRotator ShotRecoil = FRotator(
                -FMath::RandRange(0.1f, 0.2f),
                FMath::RandRange(0.1f, 0.2f), 0.f) * Recoil;
        OwningCharacter->ApplyRecoil(ShotRecoil);
    }

    // Call the fire event
    OwningCharacter->OnFired();
}

void AGunBase::ServerFire_Implementation(FVector LaunchVelocity)
{
    // Run fire on server only
    Fire(LaunchVelocity);
}

void AGunBase::MulticastFire_Implementation(FVector LaunchVelocity)
{
    // Run fire on clients only
    if (!HasAuthority()) { Fire(LaunchVelocity); }
}

void AGunBase::Reload()
{
    // Reload on the server only
    if (HasAuthority())
    {
        CurrentAmmo = MagazineSize;
    }
}

bool AGunBase::HasAmmo() const
{
    return CurrentAmmo > 0;
}

bool AGunBase::FullyLoaded() const
{
    return CurrentAmmo >= MagazineSize;
}

int32 AGunBase::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

int32 AGunBase::GetMagazineSize() const
{
    return MagazineSize;
}