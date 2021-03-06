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

public:

	// Public overrides

    virtual void Init();

    // Public methods

    void SetServerList(class UServerListWidget* Caller);

    void CreateSession();

    void SearchSessions();

    void JoinFoundSession(FOnlineSessionSearchResult& SessionResult);

private:

    // Private fields

    UServerListWidget* ServerListWidget;

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
