// Caps Collective 2020


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../HUDs/MenuHUD.h"

bool UMainMenuWidget::Initialize()
{
    bool bReturnValue = Super::Initialize();

    // Setup widget bindings
    ButtonPlay->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayButtonPressed);
    ButtonQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitButtonPressed);

    return bReturnValue;
}

void UMainMenuWidget::OnPlayButtonPressed()
{
    // Open the server list
    AMenuHUD* HUD = GetWorld()->GetFirstPlayerController()->GetHUD<AMenuHUD>();
    if (HUD)
    {
        HUD->OpenServerList();
    }
}

void UMainMenuWidget::OnQuitButtonPressed()
{
    // Quit the application
    UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
                                   EQuitPreference::Quit, false);
}