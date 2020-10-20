// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerLevelController.generated.h"


UCLASS()
class RUNFORCOVERT_API APlayerLevelController : public APlayerController
{
	GENERATED_BODY()

public:

    // Public functions

    void HideHUD();

protected:

    // Protected overrides

    virtual void SetupInputComponent() override;

private:

    // Private functions

    void OpenMenu();

};
