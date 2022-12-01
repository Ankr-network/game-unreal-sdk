// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MirageClient.h"
#include "UpdateNFTExample.h"
#include "WearableNFTExample.h"
#include "AdvertisementManager.h"
#include "MirageDelegates.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIRAGEUNREALSDK_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "MIRAGE SDK")
	FApplicationResume onApplicationResume;

	FDelegateHandle hndl_AppWillEnterBackground;
	FDelegateHandle hndl_AppHasEnteredForground;
	FDelegateHandle hndl_ApplicationWillDeactivate;
	bool lostFocus;

	UMyGameInstance();
	~UMyGameInstance();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "MIRAGE SDK")
	UMirageClient* mirageClient;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "MIRAGE SDK")
	UUpdateNFTExample* updateNFTExample;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "MIRAGE SDK")
	UWearableNFTExample* wearableNFTExample;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "MIRAGE SDK")
	UAdvertisementManager* advertisementManager;

	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	UMirageClient* GetMirageClient();

	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	UUpdateNFTExample* GetUpdateNFTExample();

	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	UWearableNFTExample* GetWearableNFTExample();

	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	UAdvertisementManager* GetAdvertisementManager();

	void OnMobileFocusLost();
	void OnMobileFocusGained();
	void OnMobileAppDeactivated();

	UFUNCTION()
		void OnApplicationResume();
};
