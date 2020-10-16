// Caps Collective 2020


#include "../Characters/CharacterBase.h"
#include "GunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"

AGunBase::AGunBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
    GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
    RootComponent = GunMesh;

    MuzzlePosition = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Position"));
    MuzzlePosition->SetupAttachment(GunMesh);

    // Set field default values
    Character = nullptr;
    FireSound = nullptr;
    ClipEmptySound = nullptr;
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

void AGunBase::BeginPlay()
{
    Super::BeginPlay();

    // Set the magazine to full
    CurrentAmmo = MagazineSize;
    Character = Cast<ACharacterBase>(GetParentActor());
}

void AGunBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bTriggerDown && Character)
    {
        Fire(Character->GetAimVector());
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

void AGunBase::Fire(FVector LaunchDirection)
{
    if (!Character->GetController()) { return; }

    // Check if the gun is within max fire rate and update
    if (LastFireTime + MaxFireRate > GetWorld()->GetTimeSeconds()) { return; }
    LastFireTime = GetWorld()->GetTimeSeconds();

    // Check if there is enough available ammunition
    if (CurrentAmmo <= 0) { return; }

    // Decrement ammunition and play firing sound
    --CurrentAmmo;
    UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound,
                                           MuzzlePosition->GetComponentLocation());

    // Get projectile trace
    FPredictProjectilePathResult Result;
    FPredictProjectilePathParams Params = FPredictProjectilePathParams(
            1.f,
            MuzzlePosition->GetComponentLocation(),
            FMath::VRand() * BulletSpread + LaunchDirection * BulletSpeed,
            2.f,
            ECC_WorldDynamic,
            GetParentActor()
    );
    if (UGameplayStatics::PredictProjectilePath(GetWorld(), Params, OUT Result))
    {
        // Damage actor
        AActor* HitActor = Result.HitResult.GetActor();
        if (!HitActor) { return; }
        FPointDamageEvent DamageEvent = FPointDamageEvent(GunDamage, Result.HitResult,
                                                          Result.HitResult.ImpactNormal, nullptr);
        HitActor->TakeDamage(GunDamage,DamageEvent, Character->GetController(),GetOwner());
    }

    // Draw debug line
    for (auto It = Result.PathData.CreateConstIterator(); It; It++)
    {
        DrawDebugPoint(
                GetWorld(), (*It).Location,5.f,
                It + 1 ? FColor::Green : FColor::Red,false,4.f
        );
    }

    // Apply recoil to the character and call the fire event
    FRotator ShotRecoil = FRotator(
            -FMath::RandRange(0.1f, 0.2f),
            FMath::RandRange(0.1f, 0.2f), 0.f) * Recoil;
    Character->ApplyRecoil(ShotRecoil);
    Character->OnFired();
}

bool AGunBase::HasAmmo() const
{
    return CurrentAmmo > 0;
}

bool AGunBase::FullyLoaded() const
{
    return CurrentAmmo < MagazineSize;
}

void AGunBase::Reload()
{
    CurrentAmmo = MagazineSize;
}

int32 AGunBase::GetCurrentAmmo() const
{
    return CurrentAmmo;
}