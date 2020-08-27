// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CoverNode.generated.h"

/**
 * While this would ideally be a struct, Unreal hates struct pointers
 * with a passion.
 */
UCLASS()
class RUNFORCOVERT_API UCoverNode : public UObject
{

	GENERATED_BODY()

public:

    // Public fields

    UPROPERTY()
    class ACover* CoverActor;

    class UCoverPositionComponent* ValidCover;
    UPROPERTY()
    UCoverNode* CameFrom;

    UPROPERTY()
    TArray<UCoverNode*> AdjacentCover;

    float GScore;

    float HScore;

    // Public functions

    float FScore();
	
};
