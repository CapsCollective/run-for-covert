// Caps Collective 2020


#include "GunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Components/HealthComponent.h"
#include "DrawDebugHelpers.h"

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

void AGunBase::Fire(FVector LaunchDirection)
{
    // Play firing sound
    UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, MuzzlePosition->GetComponentLocation());

    // Get projectile trace
    FPredictProjectilePathResult Result;
    FPredictProjectilePathParams Params = FPredictProjectilePathParams(
            1.f,
            MuzzlePosition->GetComponentLocation(),
            LaunchDirection * 10000.f,
            2.f,
            ECC_WorldDynamic
    );
    if (UGameplayStatics::PredictProjectilePath(GetWorld(), Params, OUT Result))
    {
        UE_LOG(LogTemp, Error, TEXT("Hit something!"))
        DrawDebugLine(GetWorld(), MuzzlePosition->GetComponentLocation(), Result.HitResult.ImpactPoint, FColor::Red, true);

        // Damage actor
        AActor* HitActor = Result.HitResult.GetActor();
        if (!HitActor) { return; }

        UHealthComponent* HealthComponent = Cast<UHealthComponent>(
                HitActor->GetComponentByClass(TSubclassOf<UHealthComponent>()));

        if (!HealthComponent) { return; }

        HealthComponent->OnTakeDamage(GunDamage);
    }
}