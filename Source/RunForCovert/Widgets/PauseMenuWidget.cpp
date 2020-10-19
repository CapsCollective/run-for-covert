// Caps Collective 2020


#include "PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

bool UPauseMenuWidget::Initialize()
{
    bool bReturnValue = Super::Initialize();

    // Setup widget bindings
    ButtonMenu->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMenuButtonPressed);

    return bReturnValue;
}

void UPauseMenuWidget::OnMenuButtonPressed()
{
    // Return to the main menu
    // TODO make sure this disconnects from the session (and destroys it)
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}