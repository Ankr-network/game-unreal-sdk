#include "AdvertisementManager.h"
#include "AnkrUtility.h"
#include <string>

UAdvertisementManager::UAdvertisementManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("UAdvertisementManager - Constructor"));
    
#if PLATFORM_IOS
    //if(libraryManageriOS == nullptr)
    //{
        //libraryManageriOS = [[LibraryManager alloc] init];
    //}
#elif PLATFORM_ANDROID
	//LibraryManager::GetInstance().Load();
#endif
}

void UAdvertisementManager::InitializeAdvertisement(FString _deviceId, FString _appId, FString _publicAddress, FString _language)
{
#if PLATFORM_IOS
    /*std::string _deviceIdStr = std::string(TCHAR_TO_UTF8(*_deviceId));
    NSString* _deviceIdNSString = [[NSString alloc] initWithUTF8String:_deviceIdStr.c_str()];
    
    std::string _appIdStr = std::string(TCHAR_TO_UTF8(*_appId));
    NSString* _appIdNSString = [[NSString alloc] initWithUTF8String:_appIdStr.c_str()];
    
    std::string _publicAddressStr = std::string(TCHAR_TO_UTF8(*_publicAddress));
    NSString* _publicAddressNSString = [[NSString alloc] initWithUTF8String:_publicAddressStr.c_str()];
    
    std::string _languageStr = std::string(TCHAR_TO_UTF8(*_language));
    NSString* _languageNSString = [[NSString alloc] initWithUTF8String:_languageStr.c_str()];
    
    [libraryManageriOS Initialize:_appIdNSString deviceId:_deviceIdNSString publicAddress:_deviceIdNSString language:_languageNSString];*/
#elif PLATFORM_ANDROID



#else
    
	deviceId = _deviceId;
	appId = _appId;
	activeAccount = _publicAddress;
	language = _language;
#endif
}

void UAdvertisementManager::LoadAd(FString _unitId)
{
#if PLATFORM_IOS
    //std::string _unitIdStr = std::string(TCHAR_TO_UTF8(*_unitId));
    //NSString* _unitIdNSString = [[NSString alloc] initWithUTF8String:_unitIdStr.c_str()];
    
    //[libraryManageriOS LoadAd:_unitIdNSString];
#else
    
    
#endif
}

void UAdvertisementManager::Show(FString _unitId)
{
#if PLATFORM_IOS
    //std::string _unitIdStr = std::string(TCHAR_TO_UTF8(*_unitId));
    //NSString* _unitIdNSString = [[NSString alloc] initWithUTF8String:_unitIdStr.c_str()];
    
    //[libraryManageriOS Show:_unitIdNSString];
#else
    
    
#endif
}

void UAdvertisementManager::StartSession()
{
	/*http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - StartSession - GetContentAsString: %s"), *content);
		});

	FString url = UAnkrUtility::GetAdUrl() + ENDPOINT_START_SESSION;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"app_id\": \"" + appId + "\", \"device_id\": \"" + deviceId + "\", \"public_address\":\"" + activeAccount + "\", \"language\":\"" + language + "\"}");
	Request->ProcessRequest();*/

	const FString payload = FString("{\"app_id\": \"" + appId + "\", \"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"public_address\":\"" + UAnkrUtility::GetWalletAddress() + "\", \"language\":\"" + language + "\"}");
	
	SendRequest(UAnkrUtility::GetAdUrl() + ENDPOINT_START_SESSION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - StartSession: %s"), *content);
		}, false);
}

void UAdvertisementManager::GetAdvertisement(FString _unit_id, const FAdvertisementReceivedDelegate& advertisementData)
{
	/*http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([advertisementData, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - GetAdvertisement - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			FString data = content;
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FAdvertisementDataStructure adData = FAdvertisementDataStructure::FromJson(data);
				adData.result.texture_url = UAnkrUtility::GetAdUrl() + ENDPOINT_AD + SLASH + adData.result.uuid;
				adData.Log();

				advertisementData.ExecuteIfBound(adData);
			
				if (adData.code == AD_SESSION_EXPIRED)
				{
					//StartSession();
				}
			}
		});

	FString url = UAnkrUtility::GetAdUrl() + ENDPOINT_AD;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"unit_id\":\"" + _unit_id + "\"}");
	Request->ProcessRequest();*/

	const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"unit_id\":\"" + _unit_id + "\"}");

	SendRequest(UAnkrUtility::GetAdUrl() + ENDPOINT_AD, "POST", payload, [this, advertisementData](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetAdvertisement: %s"), *content);

			FAdvertisementDataStructure adData = FAdvertisementDataStructure::FromJson(content);
			adData.result.texture_url = UAnkrUtility::GetAdUrl() + ENDPOINT_AD + SLASH + adData.result.uuid;
			adData.Log();

			advertisementData.ExecuteIfBound(adData);
			
			if (adData.code == AD_SESSION_EXPIRED)
			{
				//StartSession();
			}

		}, false);
}

void UAdvertisementManager::DownloadVideoAdvertisement(FAdvertisementDataStructure advertisementData, FAdvertisementVideoAdDownloadDelegate Result)
{
	/*http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([this, advertisementData, Result](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const TArray<uint8> data = Response->GetContent();

			FString path = *FPaths::ProjectSavedDir() + FString("VideoAd/").Append(advertisementData.result.uuid).Append(".mp4");
			FFileHelper::SaveArrayToFile(data, *path);

			Result.ExecuteIfBound(path);
		});

	Request->SetURL(advertisementData.result.texture_url);
	Request->SetVerb("GET");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->ProcessRequest();*/

	SendRequest(advertisementData.result.texture_url, "GET", "", [this, advertisementData, Result](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - DownloadVideoAdvertisement: %s"), *content);

			FString path = *FPaths::ProjectSavedDir() + FString("VideoAd/").Append(advertisementData.result.uuid).Append(".mp4");
			FFileHelper::SaveArrayToFile(bytes, *path);

			Result.ExecuteIfBound(path);

		}, false);
}

void UAdvertisementManager::ShowAdvertisement(FAdvertisementDataStructure _data)
{
	/*http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - ShowAdvertisement - GetContentAsString: %s"), *content);
		});

	FString started_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());
	FString finished_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	FString url = UAnkrUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("show");
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"started_at\": \"" + started_at + "\", \"finished_at\":\"" + finished_at + "\"}");
	Request->ProcessRequest();*/

	FString started_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());
	FString finished_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	const FString url = UAnkrUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("show");
	const FString payload = FString("{\"started_at\": \"" + started_at + "\", \"finished_at\":\"" + finished_at + "\"}");

	SendRequest(url, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - ShowAdvertisement: %s"), *content);
		}, false);
}

void UAdvertisementManager::RewardAdvertisement(FAdvertisementDataStructure _data)
{
	/*http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - RewardAdvertisement - GetContentAsString: %s"), *content);
		});

	FString rewarded_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	FString url = UAnkrUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("reward");
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"rewarded_at\": \"" + rewarded_at + "\"}");
	Request->ProcessRequest();*/

	FString rewarded_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	const FString url = UAnkrUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("reward");
	const FString payload = FString("{\"rewarded_at\": \"" + rewarded_at + "\"}");

	SendRequest(url, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - RewardAdvertisement: %s"), *content);
		}, false);
}

void UAdvertisementManager::EngageAdvertisement(FAdvertisementDataStructure _data)
{
	/*http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AdvertisementManager - EngageAdvertisement - GetContentAsString: %s"), *content);
		});

	FString clicked_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	FString url = UAnkrUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("engage");
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"clicked_at\": \"" + clicked_at + "\"}");
	Request->ProcessRequest();*/

	FString clicked_at = FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp());

	const FString url = UAnkrUtility::GetAdUrl() + ENDPOINT_AD + SLASH + _data.result.uuid + SLASH + FString("engage");
	const FString payload = FString("{\"clicked_at\": \"" + clicked_at + "\"}");

	SendRequest(url, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - EngageAdvertisement: %s"), *content);
		}, false);
}
