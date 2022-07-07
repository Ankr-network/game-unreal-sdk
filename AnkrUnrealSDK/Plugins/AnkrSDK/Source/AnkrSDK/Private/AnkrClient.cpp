#include "AnkrClient.h"
#include "AnkrSaveGame.h"
#include "AnkrUtility.h"

// First of all a deviceId is generated and saved for the user. Secondly updateNFTExample and wearableNFTExample objects are instantiated.
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
}

// Ping is to make sure if we can ping the Ankr API.
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

	FString url = API_BASE_URL + ENDPOINT_PING;
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->ProcessRequest();
}

// ConnectWallet is used to connect wallet (Metamask). 
// Wallet app will be opened on mobile devices only, as on desktop (Windows/Mac) a QR Code will be generated at the time the login button is pressed. Scan the QR Code with your Wallet app from mobile.
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

	FString url = API_BASE_URL + ENDPOINT_CONNECT;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\"}");
	Request->ProcessRequest();
}

// GetWalletInfo is used to get the connected wallet account and the chainId.
// The account can be used whenever the user's public address is needed in any transactions.
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

	FString url = API_BASE_URL + ENDPOINT_WALLET_INFO;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\"}");
	Request->ProcessRequest();
}

// Returns the currently connected wallet address.
FString UAnkrClient::GetActiveAccount()
{
	return !activeAccount.IsEmpty() ? activeAccount : "";
}

// SendABI is used to get the abi hash.
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

	FString url = API_BASE_URL + ENDPOINT_ABI;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString(body);
	Request->ProcessRequest();
}

// SendTransaction is used to send a trasaction provided that the paramters are entered correctly.
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
			FString url = API_BASE_URL + ENDPOINT_SEND_TRANSACTION;
			Request->SetURL(url);
			Request->SetVerb("POST");
			Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
			Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");
			Request->ProcessRequest();
		});
}

// GetTicketResult is used to get the status of the ticket having a 'code' and 'status'.
// The 'status' shows whether the result for the ticket signed has a success or failure.
// The 'code' shows a code number related to a specific failure or success.
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

	FString url = API_BASE_URL + ENDPOINT_RESULT;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"ticket\": \"" + ticketId + "\" }");
	Request->ProcessRequest();
}

// GetData is used to get data from readable functions from the contract provided that the parameters are entered correctly.
void UAnkrClient::GetData(FString contract, FString abi_hash, FString method, FString args, const FAnkrCallCompleteDynamicDelegate& Result)
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
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetData - GetContentAsString: %s"), *content);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			Result.ExecuteIfBound(content, content, "", -1, false);
		});

	FString url = API_BASE_URL + ENDPOINT_CALL_METHOD;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");
	Request->ProcessRequest();
}

// SignMessage is used to to sign and message, the ticket will be generated.
// Metamask will show popup to sign or confirm the transaction for that ticket.
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

	FString url = API_BASE_URL + ENDPOINT_SIGN_MESSAGE;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"message\":\"" + message + "\"}");
	Request->ProcessRequest();
}

// GetSignature is used to get the result of the signed message ticket and a 'data' object with 'signature' string field will be received.
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

	FString url = API_BASE_URL + ENDPOINT_RESULT;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"ticket\":\"" + ticket + "\"}");
	Request->ProcessRequest();
}

// VerifyMessage is used to confirm whether the user signed the message, an account 'address' will be received.
// The account address will be the connected wallet address.
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

	FString url = API_BASE_URL + ENDPOINT_VERIFY_MESSAGE;
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
