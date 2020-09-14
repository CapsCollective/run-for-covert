// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

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
    void SeePlayer(AActor* ActorSensed, FAIStimulus Stimulus);

    // Public fields

    UPROPERTY()
    APawn* Player;

    UPROPERTY()
    class AEnemyCharacterBase* Agent;
    
    UPROPERTY()
    class UCoverSystem* CoverSystem;

    UPROPERTY()
    class UPatrolSystem* PatrolSystem;

    // TODO convert these fields into blackboard values

    bool bTakenValidCover;

    bool bHasFinishedFiring;

    bool bSeenPlayer;

    bool bChasingPlayer;
    
protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    UPROPERTY()
    class UStateMachine* StateMachine;

    float SeenPlayerFor;
    
};
