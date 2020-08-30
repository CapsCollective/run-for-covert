// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "CoverNode.h"
#include "CoverSystem.generated.h"

UCLASS()
class RUNFORCOVERT_API UCoverSystem : public UObject
{

	GENERATED_BODY()
	
public:

    UCoverSystem();

    // Public functions

    void Initialise(UWorld* InWorld);

    class UCoverPointComponent* FindClosestValidCoverPoint(AActor* Agent, AActor* Enemy);

    TArray<ACover*> FindCoverPath(AActor* Agent, AActor* Enemy);

private:

    // Private fields

    float CoverRadius;

    UPROPERTY()
    TArray<UCoverNode*> CoverNodes;

    UPROPERTY()
    UWorld* World;

    // Private methods

    UCoverNode* GetClosestCover(AActor* Actor, bool MustBeUnoccupied = false, AActor* OtherAgent = nullptr);

    void GenerateGraph(float Radius);

    void DisplayDebugGraph(float DisplayTime);

    void DisplayDebugPath(TArray<ACover*>* Path, float DisplayTime);

};