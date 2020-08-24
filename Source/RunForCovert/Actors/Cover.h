// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunForCovert/Components/CoverPositionComponent.h"
#include "Cover.generated.h"

UCLASS()
class RUNFORCOVERT_API ACover : public AActor
{

	GENERATED_BODY()
	
public:

	ACover();

    // Public functions

    UCoverPositionComponent* FindCover(FVector &CoverFromPosition, TArray<AActor*> &IgnoredActors);

protected:

    // Protected overrides

	virtual void BeginPlay() override;

private:

    // Private fields

    TArray<UCoverPositionComponent*> CoverPoints;

};
