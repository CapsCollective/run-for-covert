// Caps Collective 2020


#include "MenuHUD.h"
#include "Blueprint/UserWidget.h"


AMenuHUD::AMenuHUD()
{
    PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    MenuWidgetClass = UUserWidget::StaticClass();
    ServerListWidgetClass = UUserWidget::StaticClass();
    MenuWidget = nullptr;
    ServerListWidget = nullptr;
}

void AMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    // Add the widgets to the viewport
    MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
    MenuWidget->AddToViewport();

    ServerListWidget = CreateWidget<UUserWidget>(GetWorld(), ServerListWidgetClass);
    ServerListWidget->AddToViewport();
    ServerListWidget->SetVisibility(ESlateVisibility::Hidden);

    // Set the input mode to UI only and show the mouse
    FInputModeUIOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
    InputMode.SetWidgetToFocus(MenuWidget->TakeWidget());
    GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void AMenuHUD::OpenServerList()
{
    MenuWidget->SetVisibility(ESlateVisibility::Hidden);
    ServerListWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMenuHUD::OpenMenu()
{
    ServerListWidget->SetVisibility(ESlateVisibility::Hidden);
    MenuWidget->SetVisibility(ESlateVisibility::Visible);
}