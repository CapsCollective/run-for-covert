// Caps Collective 2020


#include "HUDBase.h"
#include "Blueprint/UserWidget.h"


AHUDBase::AHUDBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    HUDWidgetClass = UUserWidget::StaticClass();
    HUDWidget = nullptr;
}

void AHUDBase::BeginPlay()
{
    Super::BeginPlay();

    // Initialise the HUD widget
    HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
    HUDWidget->AddToViewport();
}

void AHUDBase::DisplayHUD(bool bShow)
{
    HUDWidget->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}