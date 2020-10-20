// Caps Collective 2020


#include "PlayerLevelController.h"
#include "../HUDs/HUDBase.h"
#include "GameFramework/Pawn.h"


void APlayerLevelController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction(TEXT("Exit"), EInputEvent::IE_Pressed, this, &APlayerLevelController::OpenMenu);
}

void APlayerLevelController::HideHUD()
{
    GetHUD<AHUDBase>()->DisplayHUD(false);
}

void APlayerLevelController::OpenMenu()
{
    if (GetPawn()->InputEnabled())
    {
        GetPawn()->DisableInput(this);
    }
    else
    {
        GetPawn()->EnableInput(this);
    }
    GetHUD<AHUDBase>()->TogglePause();
}