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
    TimeToSeePlayer = 1.5f;
    FiringRange = 500.f;
    
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
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

void AEnemyCharacterBase::SetCrouching(bool Crouching)
{
    Crouching ? Crouch() : UnCrouch();
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

void AEnemyCharacterBase::SeePlayer(AActor* ActorSensed, FAIStimulus Stimulus)
{
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
