// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "PatrolNode.h"
#include "UObject/NoExportTypes.h"
#include "PatrolSystem.generated.h"


UCLASS()
class RUNFORCOVERT_API UPatrolSystem : public UObject
{
	GENERATED_BODY()
	
	public:

	// Public functions

	void Initialise(UWorld* InWorld);

	class APatrol* FindClosestValidPatrolPoint(AActor* Agent);

	TArray<APatrol*> FindPath(AActor* Agent, AActor* Enemy);

	private:

	// Private fields

	UPROPERTY()
	TArray<UPatrolNode*> PatrolNodes;

	UPROPERTY()
	UWorld* World;

	// Private methods

	UPatrolNode* GetClosestPatrol(AActor* Actor, bool MustBeUnoccupied = false, AActor* OtherAgent = nullptr);

	void GenerateGraph(float Radius);

	void DisplayDebugGraph(float DisplayTime);

	void DisplayDebugPath(TArray<APatrol*>* Path, float DisplayTime);

};
