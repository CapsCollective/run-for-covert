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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    int32 MagazineSize;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float MaxFireRate;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float BulletSpeed;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float BulletSpread;

    UPROPERTY(EditAnywhere, Category = "Effects")
    USoundBase* FireSound;

    UPROPERTY(EditAnywhere, Category = "Effects")
    USoundBase* ClipEmptySound;

    // Public functions

    void SetTriggerDown(bool bPulled);

    void Reload();

    bool HasAmmo() const;

    UFUNCTION(BlueprintPure)
    int32 GetCurrentAmmo() const;

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Components

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* GunMesh;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* MuzzlePosition;

    // Private fields

    class ACharacterBase* Character;

    float LastFireTime;

    int32 CurrentAmmo;

    bool bTriggerDown;

    // Private functions

    void Fire(FVector LaunchDirection);

};
