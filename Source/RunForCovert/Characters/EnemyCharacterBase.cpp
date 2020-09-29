// Caps Collective 2020


#include "EnemyCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AEnemyCharacterBase::AEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup components
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

    // Setup the perception sight config
    SightConfig->SightRadius = 1000.0f;
    SightConfig->PeripheralVisionAngleDegrees = 60.0f;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    PerceptionComponent->ConfigureSense(*SightConfig);

    // Set field default values
    TimeToSeePlayer = 1.5f;
    FiringRange = 500.f;
}

void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacterBase::SetCrouching(bool Crouching)
{
    // Set the crouching field
    Crouching ? Crouch() : UnCrouch();
}

bool AEnemyCharacterBase::FireWeapon()
{
    return Fire();
}
