// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelGameMode.generated.h"

UCLASS()
class RUNFORCOVERT_API ALevelGameMode : public AGameModeBase
{

	GENERATED_BODY()

public:

    ALevelGameMode();

    // Public fields

    UPROPERTY(EditAnywhere, Category = "Classes")
    TSubclassOf<APawn> DefaultEnemyPawnClass;

    // Public functions

    class UCoverSystem* GetCoverSystem();

    class UPatrolSystem* GetPatrolSystem();

    class ALevelGenerator* GetLevelGenerator();

    UFUNCTION()
    void InitialiseSystems();

protected:

    // Protected overrides

    virtual void BeginPlay() override;

private:

    // Private fields

    UPROPERTY()
    UCoverSystem* CoverSystem;

    UPROPERTY()
    UPatrolSystem* PatrolSystem;

    UPROPERTY()
    ALevelGenerator* LevelGenerator;
};
