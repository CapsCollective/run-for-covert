// Caps Collective 2020


#include "EnemyCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"


AEnemyCharacterBase::AEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    TimeToSeePlayer = 1.5f;
    FiringRange = 500.f;
    TurningSpeed = 2.0f;
}

void AEnemyCharacterBase::SetCrouching(bool Crouching)
{
    // Set the crouching field
    Crouching ? Crouch() : UnCrouch();
}

void AEnemyCharacterBase::Fire()
{
    if (!GetGun()) { return; }

    if (GetGun()->HasAmmo())
    {
        Super::Fire();
    }
    else if (!IsReloading())
    {
        BeginReload();
    }
}
