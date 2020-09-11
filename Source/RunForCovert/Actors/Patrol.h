// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunForCovert/Objects/PatrolNode.h"

#include "Patrol.generated.h"

UCLASS()
class RUNFORCOVERT_API APatrol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrol();

	UPROPERTY(VisibleAnywhere)
	UPatrolNode* Node;

	UPROPERTY(EditAnywhere)
	TArray<APatrol*> AdjacentNodes;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	void AddNode(UPatrolNode* NewNode);
	
private:
	
};
