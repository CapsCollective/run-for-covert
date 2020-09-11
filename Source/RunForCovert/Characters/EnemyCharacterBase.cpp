// Caps Collective 2020


#include "EnemyCharacterBase.h"


#include "PlayerCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Damage.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
    TimeToSeePlayer = 1.5f;
    FiringRange = 500.f;

    // Sets up the Perception component
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
    // Sets up the sight config
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
    if(SightConfig){
        SightConfig->SightRadius = 1000.0f;
        SightConfig->PeripheralVisionAngleDegrees = 60.0f;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

        PerceptionComponent->ConfigureSense(*SightConfig);
    }
}

void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Set the crouching field
// Used by states to animate the character
void AEnemyCharacterBase::SetCrouching(bool Crouching)
{
    IsCrouching = Crouching;
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
    
    if(!PerceptionComponent) { UE_LOG(LogTemp, Error, TEXT("NO PERCEPTION COMPONENT FOUND"))}
    else
    {
        PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacterBase::SeePlayer);
    }
    GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // If the player has been noticed by the player for TimeToSeePlayer seconds
    // They will flip the boolean changing the state from Patrol to Combat
    if(bSeePlayer)
    {
        if(SeenPlayerFor < TimeToSeePlayer)
        {
            SeenPlayerFor += DeltaTime;
        }
        else
        {
            // Chase Player
            bChasePlayer = true;
        }
    }
    else
    {
        if(SeenPlayerFor > 0.0f)
            SeenPlayerFor -= DeltaTime;
    }
}

bool AEnemyCharacterBase::FireWeapon()
{
    return Fire();
}

// The Perception Component delegate
void AEnemyCharacterBase::SeePlayer(AActor* ActorSensed, FAIStimulus Stimulus)
{
    // Checks if the detected Stimulus is the player or not
    // If it is, check bSeePlayer to true
    APlayerCharacterBase* pcb = Cast<APlayerCharacterBase>(ActorSensed);
    if(pcb)
    {
        if(Stimulus.WasSuccessfullySensed())
        {
            bSeePlayer = true;
        }
        else
        {
            bSeePlayer = false;
        }
    }
}
