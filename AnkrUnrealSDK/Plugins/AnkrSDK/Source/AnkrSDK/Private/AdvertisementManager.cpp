#include "AdvertisementManager.h"
#include "AnkrUtility.h"

UAdvertisementManager::UAdvertisementManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("UAdvertisementManager - Constructor"));
}

void UAdvertisementManager::Init(FString _deviceId, FString _session)
{
	deviceId = _deviceId;
	session = _session;
}

void UAdvertisementManager::SetAccount(FString _account, int _chainId)
{
	activeAccount = _account;
	chainId = _chainId;
}

void UAdvertisementManager::InitializeAdvertisement(FString _appId, FString _language)
{
	appId = _appId;
	language = _language;
}

void UAdvertisementManager::GetAdvertisement(EAdvertisementType advertisementType, FAdvertisementReceivedDelegate advertisementData)
{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION < 22)
	const TCHAR* enumPath = TEXT("AdvertisementData.EAdvertisementType");
	FString adType;
	UEnum::GetValueAsString(enumPath, advertisementType, adType);
#else
	const FString advertisementTypeString = StaticEnum<EAdvertisementType>()->GetValueAsString(advertisementType);

	TArray<FString> splits;
	FString seperator("::");
	advertisementTypeString.ParseIntoArray(splits, *seperator, true);
	const FString adType = splits[1];
#endif

	http = &FHttpModule::Get();

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
			adData.Log();

			advertisementData.ExecuteIfBound(adData);

			if (adData.code == 1)
			{
				StartSession();
			}
		}
	});

	FString url = API_AD_URL + ENDPOINT_AD;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"public_address\":\"" + activeAccount + "\", \"ad_type\":\"" + adType + "\"}");
	Request->ProcessRequest();
}

void UAdvertisementManager::StartSession()
{
	http = &FHttpModule::Get();

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

	FString url = API_AD_URL + ENDPOINT_START_SESSION;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"app_id\": \"" + appId + "\", \"device_id\": \"" + deviceId + "\", \"public_address\":\"" + activeAccount + "\", \"language\":\"" + language + "\"}");
	Request->ProcessRequest();
}