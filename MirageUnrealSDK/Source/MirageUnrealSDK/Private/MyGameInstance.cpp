// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	hndl_AppWillEnterBackground = FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &UMyGameInstance::OnMobileFocusLost);
	hndl_AppHasEnteredForground = FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &UMyGameInstance::OnMobileFocusGained);
}

UMirageClient* UMyGameInstance::GetMirageClient()
{
	if (mirageClient == nullptr)
	{
		mirageClient = NewObject<UMirageClient>();
	}

	return mirageClient;
}

void UMyGameInstance::OnMobileFocusLost()
{
	//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, *FString("OnMobileFocusLost"));
	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::OnMobileFocusLost"));
}

void UMyGameInstance::OnMobileFocusGained()
{
	//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, *FString("OnMobileFocusGained"));
	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::OnMobileFocusGained"));
}

UMyGameInstance::~UMyGameInstance()
{
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.Remove(hndl_AppWillEnterBackground);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.Remove(hndl_AppHasEnteredForground);
}