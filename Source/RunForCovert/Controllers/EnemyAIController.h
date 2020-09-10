// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RunForCovert/Characters/EnemyCharacterBase.h"
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

    void FireAtPlayer();

    UPROPERTY()
    APawn* Player;

    UPROPERTY()
    class AEnemyCharacterBase* Agent;
    
    UPROPERTY()
    class UCoverSystem* CoverSystem;

    UPROPERTY()
    class UPatrolSystem* PatrolSystem;

    bool bPlayerSeen();

    bool bSeePlayer();

    // Public fields

    UPROPERTY()
    class UCoverPointComponent* CoverPoint;

    UPROPERTY()
    TArray<class ACover*> CoverPath;

    UPROPERTY()
    UCoverPointComponent* PreviousCoverPoint;

    bool HasFired;

    bool TakenCover;
    
protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    UPROPERTY()
    TArray<class APatrol*> PatrolPath;

    UPROPERTY()
    class UStateMachine* StateMachine;
    
};
