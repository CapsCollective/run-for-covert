// Caps Collective 2020


#include "EnemyCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"


AEnemyCharacterBase::AEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    TimeToSeePlayer = 1.5f;
    FiringRange = 500.f;
}

void AEnemyCharacterBase::SetCrouching(bool Crouching)
{
    // Set the crouching field
    Crouching ? Crouch() : UnCrouch();
}

bool AEnemyCharacterBase::FireWeapon()
{
    if (!GetGun()) { return false; }

    if (GetGun()->GetCurrentAmmo() > 0)
    {
        return Fire();
    }
    else if (!IsReloading())
    {
        BeginReload();
    }
    return false;
}
