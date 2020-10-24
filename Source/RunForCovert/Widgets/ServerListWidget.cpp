// Caps Collective 2020


#include "ServerListWidget.h"
#include "../HUDs/MenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"


bool UServerListWidget::Initialize()
{
    bool bReturnValue = Super::Initialize();

    // Setup widget bindings
    ButtonCreateGame->OnClicked.AddDynamic(this, &UServerListWidget::OnCreateGameButtonPressed);
    ButtonBack->OnClicked.AddDynamic(this, &UServerListWidget::OnBackButtonPressed);

    return bReturnValue;
}

void UServerListWidget::OnCreateGameButtonPressed()
{
    // Open a new level
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainProcGen"), true, "listen");
}

void UServerListWidget::OnBackButtonPressed()
{
    // Return to the main menu
    AMenuHUD* HUD = GetWorld()->GetFirstPlayerController()->GetHUD<AMenuHUD>();
    if (HUD)
    {
        HUD->OpenMenu();
    }
}