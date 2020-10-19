// Caps Collective 2020


#include "MenuGameMode.h"
#include "Blueprint/UserWidget.h"


void AMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Add the menu widget to the viewport
    MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
    MenuWidget->AddToViewport();

    // Set the input mode to UI only and show the mouse
    FInputModeUIOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
    InputMode.SetWidgetToFocus(MenuWidget->TakeWidget());
    GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}