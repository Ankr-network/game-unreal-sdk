#include "AnkrClient.h"
#include "AnkrSaveGame.h"
#include "AnkrUtility.h"

UAnkrClient::UAnkrClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (UAnkrSaveGame::Load() == nullptr)
	{
		UAnkrSaveGame::Save(FGuid::NewGuid().ToString());
	}

	UAnkrSaveGame* load = UAnkrSaveGame::Load();
	deviceId = load->UniqueId;
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - AnkrSDK will use device id: %s."), *deviceId);

	if (updateNFTExample == nullptr)
	{
		updateNFTExample = NewObject<UUpdateNFTExample>();
	}
	if (wearableNFTExample == nullptr)
	{
		wearableNFTExample = NewObject<UWearableNFTExample>();
	}
	if (advertisementManager == nullptr)
	{
		advertisementManager = NewObject<UAdvertisementManager>();
	}

	AnkrUtility::SetDevelopment(true);
}

void UAnkrClient::Ping(const FAnkrCallCompleteDynamicDelegate& Result)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - Ping: %s"), *content);

			Result.ExecuteIfBound(content, "", "", -1, false);
		});

	FString url = AnkrUtility::GetUrl() + ENDPOINT_PING;
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->ProcessRequest();
}

void UAnkrClient::ConnectWallet(const FAnkrCallCompleteDynamicDelegate& Result)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - ConnectWallet - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			needLogin = false;
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				bool result = JsonObject->GetBoolField("result");
				if (result)
				{
					FString recievedUri = JsonObject->GetStringField("uri");
					FString sessionId = JsonObject->GetStringField("session");
					needLogin = JsonObject->GetBoolField("login");
					session = sessionId;
					walletConnectDeeplink = recievedUri;

					updateNFTExample->Init(deviceId, session);
					wearableNFTExample->Init(deviceId, session);

					if (needLogin)
					{
#if PLATFORM_ANDROID || PLATFORM_IOS
						AnkrUtility::SetLastRequest("ConnectWallet");
						FPlatformProcess::LaunchURL(recievedUri.GetCharArray().GetData(), NULL, NULL);
#endif
					}

					Result.ExecuteIfBound(content, "", "", -1, needLogin);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("AnkrClient - ConnectWallet - Couldn't connect, when result is false, see details:\n%s"), *content);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AnkrClient - ConnectWallet - Couldn't get a valid response, deserialization failed, see details:\n%s"), *content);
			}

});

	FString url = AnkrUtility::GetUrl() + ENDPOINT_CONNECT;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\"}");
	Request->ProcessRequest();
}

void UAnkrClient::GetWalletInfo(const FAnkrCallCompleteDynamicDelegate& Result)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetWalletInfo - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			FString data = content;
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				bool result = JsonObject->GetBoolField("result");
				if (result)
				{
					TArray<TSharedPtr<FJsonValue>> accountsObject = JsonObject->GetArrayField("accounts");

					if (accountsObject.Num() > 0)
					{
						for (int32 i = 0; i < accountsObject.Num(); i++)
						{
							accounts.Add(accountsObject[i]->AsString());
						}

						activeAccount = accounts[0];
						chainId = JsonObject->GetIntegerField("chainId");

						updateNFTExample->SetAccount(activeAccount, chainId);
						wearableNFTExample->SetAccount(activeAccount, chainId);

						data = FString("Active Account: ").Append(activeAccount).Append(" | Chain Id: ").Append(FString::FromInt(chainId));
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("AnkrClient - GetWalletInfo - Couldn't get an account, wallet is not connected: %s"), *content);
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("AnkrClient - GetWalletInfo - Couldn't get a valid response: %s"), *content);
					data = JsonObject->GetStringField("msg");
				}

				Result.ExecuteIfBound(content, data, "", -1, false);
}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AnkrClient - GetWalletInfo - Couldn't get a valid response:\n%s"), *content);
			}
	});

	FString url = AnkrUtility::GetUrl() + ENDPOINT_WALLET_INFO;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\"}");
	Request->ProcessRequest();
}

FString UAnkrClient::GetActiveAccount()
{
	return !activeAccount.IsEmpty() ? activeAccount : "";
}

void UAnkrClient::SendABI(FString abi, const FAnkrCallCompleteDynamicDelegate& Result)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - SendABI - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			FString data = content;
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				Result.ExecuteIfBound(content, JsonObject->GetStringField("abi"), "", -1, false);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AnkrClient - SendABI - Couldn't get a valid response:\n%s"), *content);
			}
		});

	const TCHAR* find = TEXT("\"");
	const TCHAR* replace = TEXT("\\\"");
	FString body = FString("{\"abi\": \"" + abi.Replace(find, replace, ESearchCase::IgnoreCase) + "\"}");

	FString url = AnkrUtility::GetUrl() + ENDPOINT_ABI;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString(body);
	Request->ProcessRequest();
}

void UAnkrClient::SendTransaction(FString contract, FString abi_hash, FString method, FString args, const FAnkrCallCompleteDynamicDelegate& Result)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - SendTransaction - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			FString data = content;
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString ticketId = JsonObject->GetStringField("ticket");
				data = ticketId;

#if PLATFORM_ANDROID || PLATFORM_IOS
				AnkrUtility::SetLastRequest("SendTransaction");
				FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AnkrClient - SendTransaction - Couldn't get a valid response:\n%s"), *content);
			}

			Result.ExecuteIfBound(content, data, "", -1, false);
		});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, contract, abi_hash, method, args]()
		{
			FString url = AnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION;
			Request->SetURL(url);
			Request->SetVerb("POST");
			Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
			Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");
			Request->ProcessRequest();
		});
}

void UAnkrClient::GetTicketResult(FString ticketId, const FAnkrCallCompleteDynamicDelegate& Result)
{
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, ticketId, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetTicketResult - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				int code = JsonObject->GetIntegerField("code");
				FString status = JsonObject->GetStringField("status");

				Result.ExecuteIfBound(content, status, "", code, false);
			}
		});

	FString url = AnkrUtility::GetUrl() + ENDPOINT_RESULT;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"ticket\": \"" + ticketId + "\" }");
	Request->ProcessRequest();
}

void UAnkrClient::CallMethod(FString contract, FString abi_hash, FString method, FString args, const FAnkrCallCompleteDynamicDelegate& Result)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - CallMethod - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			Result.ExecuteIfBound(content, content, "", -1, false);
		});

	FString url = AnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");
	Request->ProcessRequest();
}

void UAnkrClient::SignMessage(FString message, const FAnkrCallCompleteDynamicDelegate & Result)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - SignMessage - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString ticketId = JsonObject->GetStringField("ticket");

#if PLATFORM_ANDROID || PLATFORM_IOS
				AnkrUtility::SetLastRequest("SignMessage");
				FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif

				Result.ExecuteIfBound(content, ticketId, "", -1, false);
			}
		});

	FString url = AnkrUtility::GetUrl() + ENDPOINT_SIGN_MESSAGE;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"message\":\"" + message + "\"}");
	Request->ProcessRequest();
}

void UAnkrClient::GetSignature(FString ticket, const FAnkrCallCompleteDynamicDelegate& Result)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetSignature - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				TSharedPtr<FJsonObject> data = JsonObject->GetObjectField("data");

				Result.ExecuteIfBound(content, data->GetStringField("signature"), "", -1, false);
			}
		});

	FString url = AnkrUtility::GetUrl() + ENDPOINT_RESULT;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"ticket\":\"" + ticket + "\"}");
	Request->ProcessRequest();
}

void UAnkrClient::VerifyMessage(FString message, FString signature, const FAnkrCallCompleteDynamicDelegate& Result)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - VerifyMessage - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				Result.ExecuteIfBound(content, JsonObject->GetStringField("address"), "", -1, false);
			}
		});

	FString url = AnkrUtility::GetUrl() + ENDPOINT_VERIFY_MESSAGE;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"message\":\"" + message + "\", \"signature\":\"" + signature + "\"}");
	Request->ProcessRequest();
}

FString UAnkrClient::GetLastRequest()
{
	return AnkrUtility::GetLastRequest();
}

void UAnkrClient::SetLastRequest(FString _lastRequest)
{
	AnkrUtility::SetLastRequest(_lastRequest);
}

void UAnkrClient::CollectStatistics(FString _app_id, FString _device_id, FString _public_address, const FAnkrCallCompleteDynamicDelegate& Result)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const FString content = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - CollectStatistics - GetContentAsString: %s"), *content);
		});

	FString url = AnkrUtility::GetStatUrl() + ENDPOINT_STATS_COLLECT;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"app_id\": \"" + _app_id + "\", \"device_id\": \"" + _device_id + "\", \"public_address\":\"" + _public_address + "\"}");
	Request->ProcessRequest();
}