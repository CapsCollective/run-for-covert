// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"


UCLASS()
class RUNFORCOVERT_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

    // Protected overrides

    virtual bool Initialize() override;

private:

    // Private fields

    UPROPERTY(meta=(BindWidget))
    class UButton* ButtonMenu;

    // Private functions

    UFUNCTION()
    void OnMenuButtonPressed();
	
};
