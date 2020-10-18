// Caps Collective 2020


#include "HUDBase.h"
#include "Blueprint/UserWidget.h"


AHUDBase::AHUDBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    HUDWidgetClass = UUserWidget::StaticClass();
    HUDWidget = nullptr;
    PauseWidget = nullptr;
}

void AHUDBase::BeginPlay()
{
    Super::BeginPlay();

    // Initialise the HUD widgets
    HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
    PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
    HUDWidget->AddToViewport();
    PauseWidget->AddToViewport();
}

void AHUDBase::DisplayHUD(bool bShow)
{
    HUDWidget->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AHUDBase::TogglePause()
{
    bool bHidden = PauseWidget->GetVisibility() == ESlateVisibility::Hidden;
    PauseWidget->SetVisibility(bHidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}