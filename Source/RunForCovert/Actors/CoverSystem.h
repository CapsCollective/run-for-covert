// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Objects/CoverNode.h"
#include "CoverSystem.generated.h"

UCLASS()
class RUNFORCOVERT_API ACoverSystem : public AActor // TODO make inherit from UObject?
{

	GENERATED_BODY()
	
public:

	ACoverSystem();

    // Public functions

    class UCoverPointComponent* FindClosestValidCoverPoint(AActor* Agent, AActor* Enemy);

    TArray<ACover*> FindCoverPath(AActor* Agent, AActor* Enemy);

protected:

    // Protected overrides

	virtual void BeginPlay() override;

private:

    // Private fields

    float CoverRadius;

    UPROPERTY()
    TArray<UCoverNode*> CoverNodes;

    // Private methods

    UCoverNode* GetClosestCover(AActor* Actor);

    void GenerateGraph(float Radius);

    void DisplayDebugGraph(float DisplayTime);

    void DisplayDebugPath(TArray<ACover*>* Path, float DisplayTime);

};