// Caps Collective 2020


#include "EnemyCharacterBase.h"


#include "PlayerCharacterBase.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Damage.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
    TimeToSeePlayer = 1.5f;
}

void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacterBase::SetCrouching(bool Crouching)
{
    IsCrouching = Crouching;
    UE_LOG(LogTemp, Warning, TEXT("SETTING CROUCH %i"), Crouching)
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
    PerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
    
    if(!PerceptionComponent) { UE_LOG(LogTemp, Error, TEXT("NO PERCEPTION COMPONENT FOUND"))}
    else
    {
        PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacterBase::SeePlayer);
    }
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

void AEnemyCharacterBase::FireWeapon()
{
    Fire();
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
