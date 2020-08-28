// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunForCovert/Components/CoverPointComponent.h"
#include "Cover.generated.h"

UCLASS()
class RUNFORCOVERT_API ACover : public AActor
{

	GENERATED_BODY()
	
public:

	ACover();

    // Public functions

    UCoverPointComponent* FindValidCoverPoint(FVector CoverFromPosition);

    UCoverPointComponent* GetRandomCover();

protected:

    // Protected overrides

	virtual void BeginPlay() override;

private:

    // Private fields

    UPROPERTY()
    TArray<UCoverPointComponent*> CoverPoints;

};
