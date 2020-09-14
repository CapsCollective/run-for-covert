// Caps Collective 2020


#include "EnemyCharacterBase.h"
#include "PlayerCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"


AEnemyCharacterBase::AEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");

    // Set up the sight config
    if(SightConfig){
        SightConfig->SightRadius = 1000.0f;
        SightConfig->PeripheralVisionAngleDegrees = 60.0f;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

        PerceptionComponent->ConfigureSense(*SightConfig);
    }

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

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Register perception delegate method
    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacterBase::SeePlayer);
}

void AEnemyCharacterBase::Tick(float DeltaTime)
{
}

bool AEnemyCharacterBase::FireWeapon()
{
    return Fire();
}

void AEnemyCharacterBase::SeePlayer(AActor* ActorSensed, FAIStimulus Stimulus)
{
    // Check bSeenPlayer to true if the detected Stimulus is the player or not
    APlayerCharacterBase* pcb = Cast<APlayerCharacterBase>(ActorSensed);
    if(pcb)
    {
        bSeenPlayer = Stimulus.WasSuccessfullySensed();
    }
}
