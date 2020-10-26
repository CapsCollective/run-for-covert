// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NetworkedGameInstance.generated.h"


UCLASS()
class RUNFORCOVERT_API UNetworkedGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// Public overrides

    virtual void Init() override;

    // Public methods

    void CreateSession(FName SessionName);

    void JoinRunningSession(FName SessionName);

private:

    // Private fields

    class IOnlineSubsystem* Subsystem;

    IOnlineSessionPtr SessionInterface;

    TSharedPtr<class FOnlineSessionSearch> SessionSearch;

    TSharedPtr<class FOnlineSessionSettings> SessionSettings;

    // Private methods

    void OnCreateSessionComplete(FName SessionName, bool bSuccess);

    void OnStartSessionComplete(FName SessionName, bool bSuccess);

    void OnDestroySessionComplete(FName SessionName, bool bSuccess);

    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    void OnFindSessionsComplete(bool bWasSuccessful);
	
};
