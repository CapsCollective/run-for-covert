// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RunForCovert/Objects/CoverSystem.h"
#include "RunForCovert/Objects/PatrolSystem.h"

#include "DefaultGameModeBase.generated.h"

UCLASS()
class RUNFORCOVERT_API ADefaultGameModeBase : public AGameModeBase
{

	GENERATED_BODY()

public:

    // Public functions

    class UCoverSystem* GetCoverSystem();
    class UPatrolSystem* GetPatrolSystem();

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    UPROPERTY()
    UCoverSystem* CoverSystem;

    UPROPERTY()
    UPatrolSystem* PatrolSystem;
};
