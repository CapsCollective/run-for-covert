// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerListWidget.generated.h"


UCLASS()
class RUNFORCOVERT_API UServerListWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

    // Protected overrides

    virtual bool Initialize() override;

private:

    // Private fields

    UPROPERTY(meta=(BindWidget))
    class UButton* ButtonBack;

    UPROPERTY(meta=(BindWidget))
    UButton* ButtonCreateGame;

    // Private functions

    UFUNCTION()
    void OnCreateGameButtonPressed();

    UFUNCTION()
    void OnBackButtonPressed();
	
};
