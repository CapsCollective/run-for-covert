// Caps Collective 2020


#include "PlayerLevelController.h"
#include "../HUDs/LevelHUD.h"
#include "GameFramework/Pawn.h"


void APlayerLevelController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction(TEXT("Exit"), EInputEvent::IE_Pressed, this, &APlayerLevelController::OpenMenu);
}

void APlayerLevelController::HideHUD()
{
    GetHUD<ALevelHUD>()->DisplayHUD(false);
}

void APlayerLevelController::OpenMenu()
{
    if (GetPawn())
    {
        // Toggle the player pawn input for the menu
        if (GetPawn()->InputEnabled())
        {
            GetPawn()->DisableInput(this);
        }
        else
        {
            GetPawn()->EnableInput(this);
        }
    }
    GetHUD<ALevelHUD>()->TogglePause();
}