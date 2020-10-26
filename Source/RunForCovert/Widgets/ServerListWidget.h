// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "ServerListWidget.generated.h"


UCLASS()
class RUNFORCOVERT_API UServerListWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    // Public methods

    void PopulateServerList(TArray<FOnlineSessionSearchResult>& Sessions);

    void DisplayMessage(const FString& Message);

protected:

    // Protected overrides

    virtual bool Initialize() override;

private:

    // Private fields

    UPROPERTY(meta=(BindWidget))
    class UButton* ButtonBack;

    UPROPERTY(meta=(BindWidget))
    UButton* ButtonSearch;

    UPROPERTY(meta=(BindWidget))
    UButton* ButtonCreate;

    UPROPERTY(meta=(BindWidget))
    class UListView* ListServers;

    UPROPERTY(meta=(BindWidget))
    UButton* ButtonJoin;

    UPROPERTY(meta=(BindWidget))
    class UTextBlock* TextDebug;

    // Private functions

    UFUNCTION()
    void OnCreateButtonPressed();

    UFUNCTION()
    void OnSearchButtonPressed();

    UFUNCTION()
    void OnJoinButtonPressed();

    UFUNCTION()
    void OnBackButtonPressed();
	
};
