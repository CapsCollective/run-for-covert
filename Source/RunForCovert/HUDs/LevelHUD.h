// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LevelHUD.generated.h"


UCLASS()
class RUNFORCOVERT_API ALevelHUD : public AHUD
{

	GENERATED_BODY()

public:

    ALevelHUD();

    // Public fields

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> HUDWidgetClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> PauseWidgetClass;

    // Public functions

	void DisplayHUD(bool bShow);

    void TogglePause();

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    UUserWidget* HUDWidget;

    UUserWidget* PauseWidget;
	
};
