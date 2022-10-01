#pragma once

#include "AnkrClientBase.h"

#if PLATFORM_IOS
#import "../Private/iOS/LibraryManager.h"
#elif PLATFORM_ANDROID
#import "../Private/Android/LibraryManager.h"
#endif

#include "AdvertisementManager.generated.h"

#define AD_SUCCESS			  0	   // Success, show the ad.
#define AD_SESSION_EXPIRED	  1    // Session expired, call /start first.
#define AD_NO_DEVICE_ID		  1001 // No device id provided.
#define AD_NO_UNIT_ID		  1002 // Unit id not found.
#define AD_DEVICE_NOT_FOUND   1003 // Ad id expired
#define AD_NO_AD_FOUND	      1004 // No suitable ad found.
#define AD_INCORRECT_APP_TYPE 1005 // Incorrect app_type.

UCLASS(Blueprintable, BlueprintType)
class ANKRSDK_API UAdvertisementManager : public UAnkrClientBase
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString appId;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString language;

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void StartSession();

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void InitializeAdvertisement(FString _appId, FString _language);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetAdvertisement(FString _unit_id, EAdvertisementTextureType _textureType, const FAdvertisementReceivedDelegate& advertisementData);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void DownloadVideoAdvertisement(FAdvertisementDataStructure advertisementData, const FAdvertisementVideoAdDownloadDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void ShowAdvertisement(FAdvertisementDataStructure _data);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void CancelAdvertisement(FAdvertisementDataStructure _data);
	
	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void FinishAdvertisement(FAdvertisementDataStructure _data);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void RewardAdvertisement(FAdvertisementDataStructure _data);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void EngageAdvertisement(FAdvertisementDataStructure _data);
    
    UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
    void LoadAd(FString _unitId);
    
    UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
    void ShowView(FString _unitId);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	FString GetAdTextureType(EAdvertisementTextureType _adTextureType);
    
#if PLATFORM_IOS
    //LibraryManager* libraryManageriOS;
#endif
};
