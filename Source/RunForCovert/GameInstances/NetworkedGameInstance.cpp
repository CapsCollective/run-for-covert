// Caps Collective 2020


#include "NetworkedGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "../HUDs/MenuHUD.h"
#include "../Widgets/ServerListWidget.h"


void UNetworkedGameInstance::Init()
{
    Super::Init();

    // Find the online subsystem's session interface and bind its delegates
    SessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();
    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetworkedGameInstance::OnFindSessionsComplete);
    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetworkedGameInstance::OnCreateSessionComplete);
    SessionInterface->OnStartSessionCompleteDelegates.AddUObject(this, &UNetworkedGameInstance::OnStartSessionComplete);
    SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNetworkedGameInstance::OnDestroySessionComplete);
    SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetworkedGameInstance::OnJoinSessionComplete);
}

void UNetworkedGameInstance::CreateSession(FName SessionName)
{
    if (SessionInterface.IsValid())
    {
        // Set session settings and begin creating the session
        SessionSettings = MakeShareable(new FOnlineSessionSettings());
        SessionSettings->bIsLANMatch = true;
        SessionSettings->bShouldAdvertise = true;
        SessionSettings->NumPublicConnections = 3;
        SessionSettings->NumPrivateConnections = 3;
        SessionSettings->bAllowJoinInProgress = true;
        SessionInterface->CreateSession(*GetFirstGamePlayer()->GetPreferredUniqueNetId(),
                                        SessionName, *SessionSettings);
    }
}

void UNetworkedGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
    if (SessionInterface.IsValid() && bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session Created Successfully"))

        // Begin starting the session
        SessionInterface->StartSession(SessionName);
    }
}

void UNetworkedGameInstance::OnStartSessionComplete(FName SessionName, bool bSuccess)
{
    if (bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session Started Successfully"))
        // Open the map level using the listen option
        UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainProcGen"), true, "listen");
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not start session"))
    }
}

void UNetworkedGameInstance::SearchSessions(UServerListWidget* Caller)
{
    if (SessionInterface.IsValid())
    {
        ServerListWidget = Caller;

        // Set session search parameters and begin searching sessions
        SessionSearch = MakeShareable(new FOnlineSessionSearch());
        SessionSearch->bIsLanQuery = true;
        SessionSearch->MaxSearchResults = 20;
        SessionSearch->PingBucketSize = 50;
        TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
        SessionInterface->FindSessions(*GetFirstGamePlayer()->GetPreferredUniqueNetId(), SearchSettingsRef);
    }
}

void UNetworkedGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (bWasSuccessful && SessionInterface.IsValid() && SessionSearch.IsValid())
    {
        // Populate the server list with search results
        ServerListWidget->PopulateServerList(SessionSearch->SearchResults);
    }
}

void UNetworkedGameInstance::JoinFoundSession(FOnlineSessionSearchResult& SessionResult)
{
    if (SessionInterface.IsValid())
    {
        // Begin joining the specified session
        SessionInterface->JoinSession(*GetFirstGamePlayer()->GetPreferredUniqueNetId(),
                                      *SessionResult.Session.GetSessionIdStr(), SessionResult);
    }
}

void UNetworkedGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (SessionInterface.IsValid() && Result == EOnJoinSessionCompleteResult::Success)
    {
        ServerListWidget->DisplayMessage(TEXT("Session joined successfully"));

        // Attempt to resolve the session connect string
        FString TravelURL;
        if (SessionInterface->GetResolvedConnectString(SessionName, OUT TravelURL))
        {
            // Client travel to the connect string
            GetFirstLocalPlayerController()->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
        }
        else
        {
            ServerListWidget->DisplayMessage(TEXT("Could not resolve connection string"));
        }
    }
    else
    {
        ServerListWidget->DisplayMessage(TEXT("Couldn't join session"));
    }
}

void UNetworkedGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
    // TODO implement this
}