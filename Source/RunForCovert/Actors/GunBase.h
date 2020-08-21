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

    UPROPERTY(EditAnywhere)
    float GunDamage;

    // Public functions

    void Fire(FVector LaunchDirection);

protected:

	virtual void BeginPlay() override;

private:

    // Components

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* GunMesh;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* MuzzlePosition;

};
