// Caps Collective 2020


#include "GunBase.h"
#include "../Characters/CharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"
#include "Net/UnrealNetwork.h"

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

    DOREPLIFETIME(AGunBase, CurrentAmmo);
}

void AGunBase::BeginPlay()
{
    Super::BeginPlay();

    // Register as the character's gun
    OwningCharacter = Cast<ACharacterBase>(GetParentActor());
    if (OwningCharacter)
    {
        OwningCharacter->SetGun(this);
        SetOwner(OwningCharacter);
    }

    // Set the magazine to full
    CurrentAmmo = MagazineSize;
}

void AGunBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bTriggerDown && OwningCharacter)
    {
        // Calculate launch velocity
        FVector LaunchVelocity = FMath::VRand() * BulletSpread + OwningCharacter->GetAimVector() * BulletSpeed;

        // Run fire on the local machine and then forward to relevant partner type
        Fire(LaunchVelocity);
        HasAuthority() ? MulticastFire(LaunchVelocity) : ServerFire(LaunchVelocity);

        // Reset the trigger for non-automatic weapons
        if (!bAutomatic) { bTriggerDown = false; }
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

void AGunBase::Fire(FVector LaunchVelocity)
{
    if (!OwningCharacter) { return; }

    // Check if the gun can fire based on ammo and fire time
    if ((LastFireTime + MaxFireRate > GetWorld()->GetTimeSeconds()) || (CurrentAmmo <= 0)) { return; }

    // Decrement ammunition and set the last fire time
    if (HasAuthority())
    {
        --CurrentAmmo;
        LastFireTime = GetWorld()->GetTimeSeconds();
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

bool AGunBase::HasAmmo() const
{
    return CurrentAmmo > 0;
}

bool AGunBase::FullyLoaded() const
{
    return CurrentAmmo >= MagazineSize;
}

void AGunBase::Reload()
{
    CurrentAmmo = MagazineSize;
}

int32 AGunBase::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

int32 AGunBase::GetMagazineSize() const
{
    return MagazineSize;
}