// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


UCLASS()
class RUNFORCOVERT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

    // Protected overrides

    virtual bool Initialize() override;

private:

    // Private fields

    UPROPERTY(meta=(BindWidget))
    class UButton* ButtonPlay;

    UPROPERTY(meta=(BindWidget))
    UButton* ButtonQuit;

    // Private functions

    UFUNCTION()
    void OnPlayButtonPressed();

    UFUNCTION()
    void OnQuitButtonPressed();
	
};
