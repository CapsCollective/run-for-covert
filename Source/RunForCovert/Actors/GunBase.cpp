// Caps Collective 2020


#include "GunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"

AGunBase::AGunBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup components
    GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
    RootComponent = GunMesh;

    MuzzlePosition = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Position"));
    MuzzlePosition->SetupAttachment(GunMesh);

    // Set field default values
    GunDamage = 10.f;
    MaxFireRate = .5f;
    LastFireTime = 0.f;
}

bool AGunBase::Fire(AController* Controller, FVector LaunchDirection)
{
    if (!Controller) { return false; }

    // Check if the gun is within max fire rate and update
    if (LastFireTime + MaxFireRate > GetWorld()->GetTimeSeconds()) { return false; }
    LastFireTime = GetWorld()->GetTimeSeconds();

    // Play firing sound
    UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, MuzzlePosition->GetComponentLocation());

    // Get projectile trace
    FPredictProjectilePathResult Result;
    FPredictProjectilePathParams Params = FPredictProjectilePathParams(
            1.f,
            MuzzlePosition->GetComponentLocation(),
            LaunchDirection * 10000.f,
            2.f,
            ECC_WorldDynamic,
            GetParentActor()
    );
    if (UGameplayStatics::PredictProjectilePath(GetWorld(), Params, OUT Result))
    {
        // Damage actor
        AActor* HitActor = Result.HitResult.GetActor();
        if (!HitActor) { return true; }
        FPointDamageEvent DamageEvent = FPointDamageEvent(GunDamage, Result.HitResult,
                                                          Result.HitResult.ImpactNormal, nullptr);
        HitActor->TakeDamage(GunDamage,DamageEvent, Controller,GetOwner());
    }

    // Draw debug line
    for (auto It = Result.PathData.CreateConstIterator(); It; It++)
    {
        DrawDebugPoint(
                GetWorld(), (*It).Location,5.f,
                It + 1 ? FColor::Green : FColor::Red,false,4.f
        );
    }

    return true;
}