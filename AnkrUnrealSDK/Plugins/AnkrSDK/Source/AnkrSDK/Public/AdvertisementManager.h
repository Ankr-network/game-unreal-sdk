#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnkrDelegates.h"

#if PLATFORM_IOS
#import "../Private/iOS/LibraryManager.h"
#endif

#include "AdvertisementManager.generated.h"

#define AD_SUCCESS			  0	   // Success, show the ad.
#define AD_NO_DEVICE_ID		  1001 // No device id provided.
#define AD_NO_APP_KEY		  1002 // No application key provided.
#define AD_INCORRECT_ADDRESS  1006 // Incorrect public_address.
#define AD_INCORRECT_LANGUAGE 1007 // Incorrect language.

#define AD_SESSION_EXPIRED	  1    // Session expired, call /start first.
#define AD_NO_UNIT_ID		  1002 // Unit id not found.
#define AD_DEVICE_NOT_FOUND   1003 // Device not found, You havn't called /start yet.
#define AD_NO_AD_FOUND	      1004 // No suitable ad found.
#define AD_INCORRECT_APP_TYPE 1005 // Incorrect app type.

UCLASS(Blueprintable, BlueprintType)
class ANKRSDK_API UAdvertisementManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	FHttpModule* http;
	FString deviceId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString appId;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString language;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString activeAccount;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) int chainId;

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void StartSession();

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void InitializeAdvertisement(FString _deviceId, FString _appId, FString _publicAddress, FString _language);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetAdvertisement(FString _unit_id, FAdvertisementReceivedDelegate advertisementData);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void DownloadVideoAdvertisement(FAdvertisementDataStructure advertisementData, FAdvertisementVideoAdDownloadDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void ShowAdvertisement(FAdvertisementDataStructure _data);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void RewardAdvertisement(FAdvertisementDataStructure _data);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void EngageAdvertisement(FAdvertisementDataStructure _data);
    
    UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
    void LoadAd(FString _unitId);
    
    UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
    void Show(FString _unitId);
    
#if PLATFORM_IOS
    LibraryManager* libraryManageriOS;
#endif
};
