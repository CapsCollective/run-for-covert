// Caps Collective 2020


#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

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
    // Return to the main menu
    // TODO make sure this opens a server browser
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainProcGen"), true, "listen");
}

void UMainMenuWidget::OnQuitButtonPressed()
{
    // Quit the application
    UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
                                   EQuitPreference::Quit, false);
}