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

    // Properties

    UPROPERTY(EditAnywhere, Category = "Combat")
    float GunDamage;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float MaxFireRate;

    UPROPERTY(EditAnywhere, Category = "Effects")
    USoundBase* FireSound;

    // Public functions

    bool Fire(AController* Controller, FVector LaunchDirection);

private:

    // Components

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* GunMesh;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* MuzzlePosition;

    // Private fields

    float LastFireTime;

};
