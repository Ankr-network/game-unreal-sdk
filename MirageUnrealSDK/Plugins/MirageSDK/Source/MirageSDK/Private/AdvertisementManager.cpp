#include "AdvertisementManager.h"
#include "MirageUtility.h"
#include "PayloadBuilder.h"
#include <string>

UAdvertisementManager::UAdvertisementManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("UAdvertisementManager - Constructor"));
    
#if PLATFORM_IOS
	LibraryManager::GetInstance().LoadLibrary();
#elif PLATFORM_ANDROID
	LibraryManager::GetInstance().LoadLibrary();
#endif
}

void UAdvertisementManager::InitializeAdvertisement(FString _appId, FString _deviceId, FString _publicAddress, FString _language)
{
	UMirageUtility::SetAdvertisementAppID(_appId);
	UMirageUtility::SetLanguage(_language);

#if PLATFORM_IOS
	LibraryManager::GetInstance().Initialize(_appId, _deviceId, _publicAddress, _language);
#elif PLATFORM_ANDROID
	LibraryManager::GetInstance().Initialize(_appId, _deviceId, _publicAddress, _language);
#endif
}

void UAdvertisementManager::LoadAd(FString _unitId)
{
#if PLATFORM_IOS
	LibraryManager::GetInstance().LoadAd(_unitId);
#elif PLATFORM_ANDROID
	LibraryManager::GetInstance().LoadAd(_unitId);
#endif
}

void UAdvertisementManager::ShowView(FString _unitId)
{
#if PLATFORM_IOS
	LibraryManager::GetInstance().ShowView(_unitId);
#elif PLATFORM_ANDROID
	LibraryManager::GetInstance().ShowView(_unitId);
#endif
}

void UAdvertisementManager::StartSession()
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"app_id",		   UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetAdvertisementAppID())},
			{"device_id",      UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID()) },
			{"public_address", UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetWalletAddress()) },
			{"language",	   UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetLanguage()) }
		});
	
	SendRequest(UMirageUtility::GetAdUrl() + ENDPOINT_START_SESSION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - StartSession: %s"), *content);
		}, false);
}

void UAdvertisementManager::GetAdvertisement(FString _unit_id, EAdvertisementTextureType _textureType, const FAdvertisementReceivedDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id",    UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID())},
			{"unit_id",      UPayloadBuilder::FStringToJsonValue(_unit_id)},
			{"texture_type", UPayloadBuilder::FStringToJsonValue(GetAdTextureType(_textureType))} 
		});

	SendRequest(UMirageUtility::GetAdUrl() + ENDPOINT_AD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAdvertisementReceivedDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - GetAdvertisement: %s"), *content);

			FAdvertisementDataStructure adData = FAdvertisementDataStructure::FromJson(content);
			adData.result.texture_url = UMirageUtility::GetAdUrl() + ENDPOINT_AD + SLASH + adData.result.uuid;
			adData.Log();
			
			callback.ExecuteIfBound(adData);

		}, Result, false);
}

void UAdvertisementManager::DownloadVideoAdvertisement(FAdvertisementDataStructure _data, const FAdvertisementVideoAdDownloadDelegate& Result)
{
	SendRequest(_data.result.texture_url, [this](const TArray<uint8> bytes, const FString content, FAdvertisementDataStructure data, const FAdvertisementVideoAdDownloadDelegate& callback)
		{
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - DownloadVideoAdvertisement: bytesLength: %d"), bytes.Num());

			FString path = *FPaths::ProjectSavedDir() + FString("VideoAd/").Append(data.result.uuid).Append(".mp4");
			FFileHelper::SaveArrayToFile(bytes, *path);

			callback.ExecuteIfBound(path);

		}, _data, Result, false);
}

void UAdvertisementManager::ShowAdvertisement(FAdvertisementDataStructure _data)
{
	FString started_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());
	FString finished_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	const FString url = UMirageUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("show");
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"started_at",  UPayloadBuilder::FStringToJsonValue("started_at")},
			{"finished_at", UPayloadBuilder::FStringToJsonValue("finished_at")} 
		});

	SendRequest(url, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - ShowAdvertisement: %s"), *content);
		}, false);
}

void UAdvertisementManager::CancelAdvertisement(FAdvertisementDataStructure _data)
{
	FString finished_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	const FString url = UMirageUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("cancel");
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"finished_at", UPayloadBuilder::FStringToJsonValue(finished_at)}
		});

	SendRequest(url, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - CancelAdvertisement: %s"), *content);
		}, false);
}

void UAdvertisementManager::FinishAdvertisement(FAdvertisementDataStructure _data)
{
	FString finished_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	const FString url = UMirageUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("finish");
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"finished_at", UPayloadBuilder::FStringToJsonValue(finished_at)}
		});

	SendRequest(url, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - FinishAdvertisement: %s"), *content);
		}, false);
}

void UAdvertisementManager::RewardAdvertisement(FAdvertisementDataStructure _data)
{
	FString rewarded_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	const FString url = UMirageUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("reward");
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"rewarded_at", UPayloadBuilder::FStringToJsonValue(rewarded_at)}
		});

	SendRequest(url, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - RewardAdvertisement: %s"), *content);
		}, false);
}

void UAdvertisementManager::EngageAdvertisement(FAdvertisementDataStructure _data)
{
	FString clicked_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	const FString url = UMirageUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("engage");
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"clicked_at", UPayloadBuilder::FStringToJsonValue(clicked_at)}
		});

	SendRequest(url, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - EngageAdvertisement: %s"), *content);
		}, false);
}

FString UAdvertisementManager::GetAdTextureType(EAdvertisementTextureType _adTextureType)
{
	switch (_adTextureType)
	{
	case EAdvertisementTextureType::E_False: return FString("false");
	case EAdvertisementTextureType::E_Video: return FString("video");
	case EAdvertisementTextureType::E_Image: return FString("image");
	default: return FString("false");
	}
}
