// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "PatrolSystem.generated.h"


UCLASS()
class RUNFORCOVERT_API UPatrolSystem : public UObject
{
	GENERATED_BODY()
	
	public:

	// Public functions

	void Initialise(UWorld* InWorld);

	class APatrolPoint* FindClosestPatrolPoint(AActor* Agent);

	private:

	// Private fields

	UPROPERTY()
	TArray<class APatrolPoint*> PatrolPoints;

	UPROPERTY()
	UWorld* World;

	// Private methods

	void DisplayDebugGraph(float DisplayTime);

};
