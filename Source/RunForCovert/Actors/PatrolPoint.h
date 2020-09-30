// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPoint.generated.h"

UCLASS()
class RUNFORCOVERT_API APatrolPoint : public AActor
{
	GENERATED_BODY()
	
public:

    // Public fields

    UPROPERTY(EditAnywhere)
    bool bSpawnsEnemy;

	UPROPERTY(EditAnywhere)
	APatrolPoint* NextPatrol;

	// Public functions

	bool TrySetNext(APatrolPoint* Patrol);
	
};
