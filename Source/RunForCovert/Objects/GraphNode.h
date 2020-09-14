// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GraphNode.generated.h"

/**
 * While this would ideally be a struct, Unreal hates struct pointers
 * with a passion.
 */
UCLASS()
class RUNFORCOVERT_API UGraphNode : public UObject
{

	GENERATED_BODY()

public:

    // Public fields

    UPROPERTY()
    AActor* Actor;

    UPROPERTY()
    UGraphNode* CameFrom;

    UPROPERTY()
    TArray<UGraphNode*> AdjacentNodes;

    float GScore;

    float HScore;

    // Public functions

    float FScore();

    template<typename T>
    T* GetActor() // Template functions must be defined in header files
    {
        return Cast<T>(Actor);
    }
	
};
