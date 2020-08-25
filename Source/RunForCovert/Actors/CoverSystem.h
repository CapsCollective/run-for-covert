// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverSystem.generated.h"

UCLASS()
class RUNFORCOVERT_API ACoverSystem : public AActor
{

	GENERATED_BODY()
	
public:

	ACoverSystem();

    // Public functions

    class UCoverPositionComponent* GetClosestValidCoverPoint(AActor* Agent, AActor* Enemy);

    TArray<UCoverPositionComponent*> GetCoverPath(UCoverPositionComponent* CurrentCoverPosition, AActor* Enemy);

protected:

    // Protected overrides

	virtual void BeginPlay() override;

private:

    // Private fields

    TArray<class ACover*> CoverActors;

    float CoverRadius;

};
