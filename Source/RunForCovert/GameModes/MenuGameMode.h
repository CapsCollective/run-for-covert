// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"


UCLASS()
class RUNFORCOVERT_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Public fields

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> MenuWidgetClass;

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    UPROPERTY()
    UUserWidget* MenuWidget;
	
};
