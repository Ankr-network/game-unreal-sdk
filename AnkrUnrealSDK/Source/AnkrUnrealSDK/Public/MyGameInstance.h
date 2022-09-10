// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
//#include "AnkrClient.h"
//#include "AnkrDelegates.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANKRUNREALSDK_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

/*public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "AnkrSDK")
	FApplicationResume onApplicationResume;

	FDelegateHandle hndl_AppWillEnterBackground;
	FDelegateHandle hndl_AppHasEnteredForground;
	FDelegateHandle hndl_ApplicationWillDeactivate;
	bool lostFocus;

	UMyGameInstance();
	~UMyGameInstance();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AnkrSDK")
	UAnkrClient* ankrClient;

	UFUNCTION(BlueprintCallable, Category = "AnkrSDK")
	UAnkrClient* GetAnkrClient();

	void OnMobileFocusLost();
	void OnMobileFocusGained();
	void OnMobileAppDeactivated();

	UFUNCTION()
		void OnApplicationResume();*/
};
