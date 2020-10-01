// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"


UCLASS()
class RUNFORCOVERT_API AHUDBase : public AHUD
{

	GENERATED_BODY()

public:

    AHUDBase();

    // Public fields

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> HUDWidgetClass;

	void DisplayHUD(bool bShow);

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    UUserWidget* HUDWidget;
	
};
