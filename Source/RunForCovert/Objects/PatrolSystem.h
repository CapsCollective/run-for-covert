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

	void Initialise();

	class APatrolPoint* FindClosestPatrolPoint(AActor* Agent, APatrolPoint* IgnoredPatrolPoint = nullptr);

	private:

	// Private fields

	UPROPERTY()
	TArray<class APatrolPoint*> PatrolPoints;

	// Private methods

	void DisplayDebugGraph(float DisplayTime);

};
