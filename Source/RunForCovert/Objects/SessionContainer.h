// Caps Collective 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlineSessionSettings.h"
#include "SessionContainer.generated.h"


UCLASS(Blueprintable)
class RUNFORCOVERT_API USessionContainer : public UObject
{
	GENERATED_BODY()

public:

    // Public fields

    UPROPERTY(BlueprintReadOnly)
    FString Name;

    FOnlineSessionSearchResult Session;
	
};
