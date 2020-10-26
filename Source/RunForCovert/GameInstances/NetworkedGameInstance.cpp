// Caps Collective 2020


#include "NetworkedGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"


void UNetworkedGameInstance::Init()
{
    Super::Init();
    Subsystem = IOnlineSubsystem::Get();

    if (Subsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found Online Subsystem: %s”"), *Subsystem->GetSubsystemName().ToString())
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Unable to find Online Subsystem"))
    }

    SessionInterface = Subsystem->GetSessionInterface();
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
        SessionSettings = MakeShareable(new FOnlineSessionSettings());
        SessionSettings->bIsLANMatch = true;
        SessionSettings->bShouldAdvertise = true;
        SessionSettings->NumPublicConnections = 3;
        SessionSettings->NumPrivateConnections = 3;
        SessionSettings->bAllowJoinInProgress = true;
        SessionInterface->CreateSession(*GetFirstGamePlayer()->GetPreferredUniqueNetId(), SessionName, *SessionSettings);
    }
}

void UNetworkedGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
    if (SessionInterface.IsValid() && bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session Created Successfully"))
        SessionInterface->StartSession(SessionName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Session is already in use, restarting session"))
        SessionInterface->DestroySession(SessionName);
    }
}

void UNetworkedGameInstance::OnStartSessionComplete(FName SessionName, bool bSuccess)
{
    if (bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session Started Successfully"))
        FInputModeGameOnly InputMode;
        InputMode.SetConsumeCaptureMouseDown(true);
        GetFirstLocalPlayerController()->bShowMouseCursor = false;
        GetFirstLocalPlayerController()->SetInputMode(InputMode);

        UGameplayStatics::OpenLevel(GetWorld(), TEXT("MultiplayerMap"), true, "listen");
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not start session"))
    }
}

void UNetworkedGameInstance::JoinRunningSession(FName SessionName)
{
    if (SessionInterface.IsValid())
    {
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
    if (SessionInterface.IsValid() && SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("%i Sessions Found"), SessionSearch->SearchResults.Num())
        if (SessionSearch->SearchResults.Num() > 0)
        {
            FOnlineSessionSearchResult SessionResult = SessionSearch->SearchResults[0];
            SessionInterface->JoinSession(*GetFirstGamePlayer()->GetPreferredUniqueNetId(),
                                          *SessionResult.Session.GetSessionIdStr(), SessionResult);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Sessions could be found"))
    }
}

void UNetworkedGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (SessionInterface.IsValid() && Result == EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session joined successfully"))
        FString TravelURL;
        if (SessionInterface->GetResolvedConnectString(SessionName, TravelURL))
        {
            UE_LOG(LogTemp, Warning, TEXT("Client travelling"))
            GetFirstLocalPlayerController()->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
        }
    }
}

void UNetworkedGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
    if (bSuccess)
    {
        CreateSession(SessionName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Session could not be created"))
        SessionInterface->DestroySession(SessionName);
    }
}