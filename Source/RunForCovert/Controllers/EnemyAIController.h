// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

UENUM()
enum class EnemySenseState : uint8 {
    DEFAULT,
    PLAYER_SEEN,
    CHASING_PLAYER,
};

UCLASS()
class RUNFORCOVERT_API AEnemyAIController : public AAIController
{

	GENERATED_BODY()

public:

    AEnemyAIController();

    // Public overrides

    virtual void Tick(float DeltaTime) override;

    // Public functions

    UFUNCTION()
    void SeePlayer(AActor* ActorSensed, const FAIStimulus& Stimulus);

    // Public fields

    UPROPERTY()
    APawn* Player;

    UPROPERTY()
    class AEnemyCharacterBase* Agent;
    
    UPROPERTY()
    class UCoverSystem* CoverSystem;

    UPROPERTY()
    class UPatrolSystem* PatrolSystem;

    UPROPERTY()
    EnemySenseState SenseState;

    // TODO convert these fields into blackboard values

    bool bTakenValidCover;

    bool bHasFinishedFiring;
    
protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    UPROPERTY()
    class UStateMachine* StateMachine;

    float SeenPlayerFor;
    
};
