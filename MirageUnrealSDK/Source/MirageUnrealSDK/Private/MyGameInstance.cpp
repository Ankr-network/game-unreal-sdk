#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	hndl_AppWillEnterBackground	   = FCoreDelegates::ApplicationWillEnterBackgroundDelegate .AddUObject(this, &UMyGameInstance::OnMobileFocusLost);
	hndl_AppHasEnteredForground	   = FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &UMyGameInstance::OnMobileFocusGained);
	hndl_ApplicationWillDeactivate = FCoreDelegates::ApplicationWillDeactivateDelegate      .AddUObject(this, &UMyGameInstance::OnMobileAppDeactivated);
	onApplicationResume.AddDynamic(this, &UMyGameInstance::OnApplicationResume);
}

UMirageClient* UMyGameInstance::GetMirageClient()
{
	if (mirageClient == nullptr)
	{
		mirageClient = NewObject<UMirageClient>();
	}

	return mirageClient;
}

UUpdateNFTExample* UMyGameInstance::GetUpdateNFTExample()
{
	if (updateNFTExample == nullptr)
	{
		updateNFTExample = NewObject<UUpdateNFTExample>();
	}

	return updateNFTExample;
}

UWearableNFTExample* UMyGameInstance::GetWearableNFTExample()
{
	if (wearableNFTExample == nullptr)
	{
		wearableNFTExample = NewObject<UWearableNFTExample>();
	}

	return wearableNFTExample;
}

UAdvertisementManager* UMyGameInstance::GetAdvertisementManager()
{
	if (advertisementManager == nullptr)
	{
		advertisementManager = NewObject<UAdvertisementManager>();
	}

	return advertisementManager;
}

void UMyGameInstance::OnMobileFocusLost()
{
	// This is called when the application is being backgrounded(e.g., due to switching to another app or closing it via the home button) The game should release shared resources, 
	// save state, etc..., since it can be terminated from the background state without any further warning.
	
	if (!lostFocus)
	{
		lostFocus = true;

		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, *FString("UMyGameInstance::OnMobileFocusLost"));
		UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::OnMobileFocusLost"));

		
	}
}

void UMyGameInstance::OnMobileFocusGained()
{
	// Called when the application is returning to the foreground(reverse any processing done in the EnterBackground delegate)

	if (lostFocus)
	{
		lostFocus = false;

		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, *FString("UMyGameInstance::OnMobileFocusGained"));
		UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::OnMobileFocusGained"));

		onApplicationResume.Broadcast();
	}
}

void UMyGameInstance::OnMobileAppDeactivated()
{
	// This is called when the application is about to be deactivated(e.g., due to a phone call or SMS or the sleep button).

	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, *FString("UMyGameInstance::OnMobileAppDeactivated"));
	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::OnMobileAppDeactivated"));
}

void UMyGameInstance::OnApplicationResume()
{
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, *FString("UMyGameInstance::OnApplicationResume"));
	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::OnApplicationResume"));
}

UMyGameInstance::~UMyGameInstance()
{
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate .Remove(hndl_AppWillEnterBackground);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.Remove(hndl_AppHasEnteredForground);
	FCoreDelegates::ApplicationWillDeactivateDelegate      .Remove(hndl_ApplicationWillDeactivate);
	onApplicationResume.RemoveDynamic(this, &UMyGameInstance::OnApplicationResume);
}