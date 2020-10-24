// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"


UCLASS()
class RUNFORCOVERT_API AMenuHUD : public AHUD
{

	GENERATED_BODY()

public:

    AMenuHUD();

    // Public fields

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> MenuWidgetClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> ServerListWidgetClass;

    // Public functions

    void OpenServerList();

    void OpenMenu();

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    UUserWidget* MenuWidget;

    UUserWidget* ServerListWidget;
	
};
