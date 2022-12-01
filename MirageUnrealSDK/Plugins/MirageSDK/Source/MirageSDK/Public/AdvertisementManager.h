#pragma once

#include "MirageClientBase.h"

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

/// AdvertisementManager provides functionalities to load and show ads for various platforms.
UCLASS(Blueprintable, BlueprintType)
class MIRAGESDK_API UAdvertisementManager : public UMirageClientBase
{
	GENERATED_UCLASS_BODY()

public:
	
	/// StartSession function is used to start a new advertisement session.
	///
	/// The function does not require parameter and returns nothing.
	/// The function is called once at the start of the app and once session gets expired.
	///
	/// @param Result A callback delegate that will be triggered once a response is received with a data.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void StartSession();

	/// InitializeAdvertisement function is used to set the app id, device id, public address and language for the ads.
	///
	/// The function requires parameters and returns nothing.
	///
	/// @param _appId The app id for the advertisement.
	/// @param _deviceId The device id that the MirageSDK is using.
	/// @param _publicAddress The wallet address of the user logged in.
	/// @param _language The language to be used for the advertisement.
	/// @attention This function also loads native ads libraries for iOS and Android.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void InitializeAdvertisement(FString _appId, FString _deviceId, FString _publicAddress, FString _language);

	/// LoadAd function is used to load the ads for the specified ad unit id.
	///
	/// The function requires a parameter and returns nothing.
	///
	/// @param _unitId The unit id of the advertisement.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void LoadAd(FString _unitId);

	/// ShowView function is used to show the ads for the specified ad unit id.
	///
	/// The function requires a parameter and returns nothing.
	///
	/// @param _unitId The unit id of the advertisement.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void ShowView(FString _unitId);

	/// GetAdvertisement function is used to get the advertisement information.
	///
	/// The function requires parameters and returns nothing.
	///
	/// @param _unit_id The unit id of the advertisement.
	/// @param _textureType Whether the type of the texture would be 'image' or 'video'.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void GetAdvertisement(FString _unit_id, EAdvertisementTextureType _textureType, const FAdvertisementReceivedDelegate& advertisementData);

	/// DownloadVideoAdvertisement function is used to download the content of the advertisement.
	///
	/// The function requires parameters and returns nothing.
	///
	/// @param advertisementData The advertisement structure that contains the download url address.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void DownloadVideoAdvertisement(FAdvertisementDataStructure advertisementData, const FAdvertisementVideoAdDownloadDelegate& Result);

	/// ShowAdvertisement function is used to show the advertisement on the screen.
	///
	/// The function requires parameters and returns nothing.
	///
	/// @param _data The advertisement data  structure that contains the information to show the ad.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void ShowAdvertisement(FAdvertisementDataStructure _data);

	/// CancelAdvertisement function is used to cancel the view ad.
	///
	/// The function requires parameters and returns nothing.
	///
	/// @param _data The advertisement data  structure that contains the information being requested to the show the ad.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void CancelAdvertisement(FAdvertisementDataStructure _data);
	
	/// FinishAdvertisement function is used to inform when the user has finished watching the ad.
	///
	/// The function requires parameters and returns nothing.
	///
	/// @param _data The advertisement data  structure that contains the information on the ads.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void FinishAdvertisement(FAdvertisementDataStructure _data);

	/// RewardAdvertisement function is used to inform when the user has finished watching rewarded ad and earned reward.
	///
	/// The function requires parameters and returns nothing.
	///
	/// @param _data The advertisement data  structure that contains the information on the ads.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void RewardAdvertisement(FAdvertisementDataStructure _data);

	/// EngageAdvertisement function is used to trace the click activities on the ad.
	///
	/// The function requires parameters and returns nothing.
	///
	/// @param _data The advertisement data  structure that contains the information on the ads.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void EngageAdvertisement(FAdvertisementDataStructure _data);

	/// GetAdTextureType function is used to open the browser when the user clicks on the ad.
	///
	/// The function requires parameters and returns FString.
	///
	/// @param _adTextureType The texture type to be selected.
	/// @return The FString texture type.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	FString GetAdTextureType(EAdvertisementTextureType _adTextureType);

};
