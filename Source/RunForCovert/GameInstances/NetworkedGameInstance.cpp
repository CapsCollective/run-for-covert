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
        ServerListWidget->DisplayMessage(TEXT("Creating session..."));
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
    if (bSuccess && SessionInterface.IsValid())
    {
        // Begin starting the session
        ServerListWidget->DisplayMessage(TEXT("Starting session..."));
        SessionInterface->StartSession(SessionName);
    }
    else if (SessionInterface.IsValid())
    {
        // Destroy the unused session
        ServerListWidget->DisplayMessage(TEXT("Destroying old sessions..."));
        SessionInterface->DestroySession(SessionName);
    }
}

void UNetworkedGameInstance::OnStartSessionComplete(FName SessionName, bool bSuccess)
{
    if (bSuccess)
    {
        // Open the map level using the listen option
        ServerListWidget->DisplayMessage(TEXT("Opening level..."));
        UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainProcGen"), true, "listen");
    }
    else
    {
        // Destroy the unused session
        ServerListWidget->DisplayMessage(TEXT("Could not start session"));
        SessionInterface->DestroySession(SessionName);
    }
}

void UNetworkedGameInstance::SearchSessions()
{
    if (SessionInterface.IsValid())
    {
        // Set session search parameters and begin searching sessions
        ServerListWidget->DisplayMessage(TEXT("Searching sessions..."));
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
        ServerListWidget->DisplayMessage(FString::Printf(TEXT("%d sessions found"), SessionSearch->SearchResults.Num()));
        ServerListWidget->PopulateServerList(SessionSearch->SearchResults);
    }
}

void UNetworkedGameInstance::JoinFoundSession(FOnlineSessionSearchResult& SessionResult)
{
    if (SessionInterface.IsValid())
    {
        // Begin joining the specified session
        ServerListWidget->DisplayMessage(TEXT("Joining session..."));
        SessionInterface->JoinSession(*GetFirstGamePlayer()->GetPreferredUniqueNetId(),
                                      *SessionResult.Session.GetSessionIdStr(), SessionResult);
    }
}

void UNetworkedGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (SessionInterface.IsValid() && Result == EOnJoinSessionCompleteResult::Success)
    {
        // Attempt to resolve the session connect string
        FString TravelURL;
        if (SessionInterface->GetResolvedConnectString(SessionName, OUT TravelURL))
        {
            // Client travel to the connect string
            GetFirstLocalPlayerController()->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
            ServerListWidget->DisplayMessage(TEXT("Client travelling..."));
        }
        else
        {
            // Destroy the unused session
            ServerListWidget->DisplayMessage(TEXT("Couldn't resolve session string"));
            SessionInterface->DestroySession(SessionName);
        }
    }
    else
    {
        // Destroy the unused session
        ServerListWidget->DisplayMessage(TEXT("Couldn't join session"));
        SessionInterface->DestroySession(SessionName);
    }
}

void UNetworkedGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
    if (!bSuccess && SessionInterface.IsValid())
    {
        ServerListWidget->DisplayMessage(TEXT("Session couldn't be destroyed"));
    }
}

void UNetworkedGameInstance::SetServerList(UServerListWidget* Widget)
{
    ServerListWidget = Widget;
}