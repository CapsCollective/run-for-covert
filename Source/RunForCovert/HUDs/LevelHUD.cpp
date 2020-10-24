// Caps Collective 2020


#include "LevelHUD.h"
#include "Blueprint/UserWidget.h"


ALevelHUD::ALevelHUD()
{
    PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    HUDWidgetClass = UUserWidget::StaticClass();
    PauseWidgetClass = UUserWidget::StaticClass();
    HUDWidget = nullptr;
    PauseWidget = nullptr;
}

void ALevelHUD::BeginPlay()
{
    Super::BeginPlay();

    // Initialise the HUD widgets
    HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
    HUDWidget->AddToViewport();

    PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
    PauseWidget->AddToViewport();
}

void ALevelHUD::DisplayHUD(bool bShow)
{
    HUDWidget->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void ALevelHUD::TogglePause()
{
    bool bHidden = PauseWidget->GetVisibility() == ESlateVisibility::Hidden;

    // Toggle the widget and mouse visibility
    PauseWidget->SetVisibility(bHidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = bHidden;
    if (bHidden)
    {
        // Also allow interaction with UI
        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
        InputMode.SetWidgetToFocus(PauseWidget->TakeWidget());
        GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
    }
    else
    {
        // Only allow interaction with the game
        GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
    }
}