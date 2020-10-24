// Caps Collective 2020


#include "MenuGameMode.h"
#include "Blueprint/UserWidget.h"


void AMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Add the menu widget to the viewport
    MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
    MenuWidget->AddToViewport();

    // Add the menu widget to the viewport
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

void AMenuGameMode::OpenServerList()
{
    MenuWidget->SetVisibility(ESlateVisibility::Hidden);
    ServerListWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMenuGameMode::OpenMenu()
{
    ServerListWidget->SetVisibility(ESlateVisibility::Hidden);
    MenuWidget->SetVisibility(ESlateVisibility::Visible);
}