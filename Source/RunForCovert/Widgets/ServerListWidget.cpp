// Caps Collective 2020


#include "ServerListWidget.h"
#include "../HUDs/MenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "../GameInstances/NetworkedGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "OnlineSessionSettings.h"
#include "../Objects/SessionContainer.h"


bool UServerListWidget::Initialize()
{
    bool bReturnValue = Super::Initialize();

    // Setup widget bindings
    ButtonCreate->OnClicked.AddDynamic(this, &UServerListWidget::OnCreateButtonPressed);
    ButtonBack->OnClicked.AddDynamic(this, &UServerListWidget::OnBackButtonPressed);
    ButtonSearch->OnClicked.AddDynamic(this, &UServerListWidget::OnSearchButtonPressed);
    ButtonJoin->OnClicked.AddDynamic(this, &UServerListWidget::OnJoinButtonPressed);

    GetWorld()->GetGameInstance<UNetworkedGameInstance>()->SetServerList(this);

    return bReturnValue;
}

void UServerListWidget::DisplayMessage(const FString& Message)
{
    // Display the message in the menu
    TextDebug->SetText(FText::FromString(Message));
}

void UServerListWidget::OnCreateButtonPressed()
{
    // Open a new session for a new game
    GetWorld()->GetGameInstance<UNetworkedGameInstance>()->CreateSession(TEXT("Game"));
}

void UServerListWidget::OnSearchButtonPressed()
{
    // Begin searching for sessions
    GetWorld()->GetGameInstance<UNetworkedGameInstance>()->SearchSessions();
}

void UServerListWidget::PopulateServerList(const TArray<FOnlineSessionSearchResult>& Sessions)
{
    // Display the number of found sessions and enable the join button if there are sessions
    ButtonJoin->SetIsEnabled(Sessions.Num() > 0);

    // Clear and repopulate the server list
    ListServers->ClearListItems();
    for (const FOnlineSessionSearchResult& Result : Sessions)
    {
        // Encapsulate the session inside a container object
        USessionContainer* NewContainer = NewObject<USessionContainer>(this);
        NewContainer->Session = Result;
        NewContainer->Name = Result.GetSessionIdStr();
        ListServers->AddItem(NewContainer);
    }
}

void UServerListWidget::OnJoinButtonPressed()
{
    // Get the session container object selected
    USessionContainer* Container = Cast<USessionContainer>(ListServers->GetSelectedItem());
    if (Container)
    {
        // Begin joining the selected session
        GetWorld()->GetGameInstance<UNetworkedGameInstance>()->JoinFoundSession(Container->Session);
    }
    else
    {
        DisplayMessage(TEXT("No session selected"));
    }
}

void UServerListWidget::OnBackButtonPressed()
{
    // Return to the main menu
    AMenuHUD* HUD = GetWorld()->GetFirstPlayerController()->GetHUD<AMenuHUD>();
    if (HUD)
    {
        HUD->OpenMenu();
    }
}