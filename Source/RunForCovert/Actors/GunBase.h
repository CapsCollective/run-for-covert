// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

UCLASS()
class RUNFORCOVERT_API AGunBase : public AActor
{

	GENERATED_BODY()
	
public:

	AGunBase();

    // Public overrides

    virtual void Tick(float DeltaTime) override;

    // Properties

    UPROPERTY(EditAnywhere, Category = "Combat")
    bool bAutomatic;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float GunDamage;

    UPROPERTY(EditAnywhere, Category = "Combat")
    int32 MagazineSize;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float MaxFireRate;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float BulletSpeed;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float BulletSpread;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float Recoil;

    UPROPERTY(EditAnywhere, Category = "Effects")
    USoundBase* FireSound;

    UPROPERTY(EditAnywhere, Category = "Effects")
    USoundBase* ClipEmptySound;

    UPROPERTY(EditAnywhere, Category = "Effects")
    USoundBase* RicochetSound;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* MuzzleFlashEffect;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* HitSurfaceEffect;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* HitCharacterEffect;

    // Public functions

    void SetTriggerDown(bool bPulled);

    void Reload();

    void OwningCharacterLookup();

    // Pure functions

    UFUNCTION(BlueprintPure)
    bool FullyLoaded() const;

    UFUNCTION(BlueprintPure)
    bool HasAmmo() const;

    UFUNCTION(BlueprintPure)
    int32 GetCurrentAmmo() const;

    UFUNCTION(BlueprintPure)
    int32 GetMagazineSize() const;

protected:

    // Protected overrides

    virtual void BeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

    // Components

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* GunMesh;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* MuzzlePosition;

    // Private fields

    UPROPERTY()
    class ACharacterBase* OwningCharacter;

    UPROPERTY()
    FTimerHandle CharacterSearchHandle;

    float LastFireTime;

    UPROPERTY(Replicated)
    int32 CurrentAmmo;

    UPROPERTY(Replicated)
    bool bCanFire;

    bool bTriggerDown;

    // Private functions

    void Fire(FVector LaunchVelocity);

    UFUNCTION(Server, Reliable)
    void ServerFire(FVector LaunchVelocity);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastFire(FVector LaunchVelocity);

};
