// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapAttachmentPoint.generated.h"

UCLASS()
class RUNFORCOVERT_API AMapAttachmentPoint : public AActor
{
	GENERATED_BODY()
	
public:	

	AMapAttachmentPoint();

	// Public functions

	// TODO add SetAttachment method

protected:

    // Protected overrides

	virtual void BeginPlay() override;

};