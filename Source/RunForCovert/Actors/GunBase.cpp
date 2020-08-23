// Caps Collective 2020


#include "GunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"

#define OUT

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
}

void AGunBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGunBase::Fire(AController* Controller, FVector LaunchDirection)
{
    if (!Controller) { return; }

    // Play firing sound
    UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, MuzzlePosition->GetComponentLocation());

    // Get projectile trace
    FPredictProjectilePathResult Result;
    FPredictProjectilePathParams Params = FPredictProjectilePathParams(
            1.f,
            MuzzlePosition->GetComponentLocation(), // TODO reconsider using muzzle as start position
            LaunchDirection * 10000.f,
            2.f,
            ECC_WorldDynamic
    );
    if (UGameplayStatics::PredictProjectilePath(GetWorld(), Params, OUT Result))
    {
        UE_LOG(LogTemp, Error, TEXT("Hit something!"))

        // TODO fix draw debug options
        //DrawDebugLine(GetWorld(), MuzzlePosition->GetComponentLocation(), Result.HitResult.ImpactPoint, FColor::Red, true);

        for (auto It = Result.PathData.CreateConstIterator(); It; It++)
        {
            FColor Colour = It.GetIndex() < Result.PathData.Num() ? FColor::Green : FColor::Red;
            DrawDebugPoint(GetWorld(), (*It).Location, 1.f, Colour);
        }

        // Damage actor
        AActor* HitActor = Result.HitResult.GetActor();
        if (!HitActor) { return; }
        FPointDamageEvent DamageEvent = FPointDamageEvent(GunDamage, Result.HitResult,
                                                          Result.HitResult.ImpactNormal, nullptr);
        HitActor->TakeDamage(GunDamage,DamageEvent, Controller,GetOwner());
    }
}