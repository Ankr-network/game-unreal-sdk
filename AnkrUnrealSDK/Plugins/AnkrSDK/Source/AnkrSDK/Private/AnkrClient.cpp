#include "AnkrClient.h"
#include "AnkrSaveGame.h"
#include "AnkrUtility.h"
#include "AdvertisementData.h"
#include "Containers/UnrealString.h"
#include "RequestBodyStructure.h"

// -----------
// Constructor
// -----------
// First of all a deviceId is generated and saved which will be associated with requests that are send to backend http://45.77.189.28:5000/{endpoint}. Secondly updateNFTExample and wearableNFTExample objects are instantiated.
UAnkrClient::UAnkrClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (UAnkrSaveGame::Load() == nullptr) UAnkrSaveGame::Save(FGuid::NewGuid().ToString());
	UAnkrSaveGame* load = UAnkrSaveGame::Load();
	deviceId = load->UniqueId;
	UE_LOG(LogTemp, Warning, TEXT("AnkrSDK initialized - deviceId: %s | ApiBaseUrl: %s"), *deviceId, *API_BASE_URL);

	if (updateNFTExample	 == nullptr) updateNFTExample     = NewObject<UUpdateNFTExample>();
	if (wearableNFTExample   == nullptr) wearableNFTExample   = NewObject<UWearableNFTExample>();
	if (advertisementManager == nullptr) advertisementManager = NewObject<UAdvertisementManager>();
}

// ----
// Ping
// ----
// Ping is a request sent at http://45.77.189.28:5000/ping to check if a response is obtained from server.
void UAnkrClient::Ping(FAnkrDelegate Result)
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

		Result.ExecuteIfBound(content);
	});

	FString url = API_BASE_URL + ENDPOINT_PING;
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->ProcessRequest();
}

// ---------
// GetClient
// ---------
// GetClient is used to send a request with 'device_id' as a raw body parameter at http://45.77.189.28:5000/connect to get a response having a 'uri' deeplink to openmetamask, a 'session' and a 'login' information if required.
// The 'uri' deeplink will only work on mobile devices as on desktop a QR Code will be generated at the time the login button is pressed.
// The session is saved to a variable for later use.
void UAnkrClient::GetClient(FAnkrConnectionStatus Status)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Status, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		const FString content = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetClient - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);
		
		needLogin = false;
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			bool result			  = JsonObject->GetBoolField("result");
			FString recievedUri	  = JsonObject->GetStringField("uri");
			FString sessionId	  = JsonObject->GetStringField("session");
			needLogin			  = JsonObject->GetBoolField("login");
			session				  = sessionId;
			walletConnectDeeplink = recievedUri;

			updateNFTExample->Init(deviceId, session);
			wearableNFTExample->Init(deviceId, session);
			advertisementManager->Init(deviceId, session);

			if (needLogin)
			{
#if PLATFORM_ANDROID
				AnkrUtility::SetLastRequest("GetClient");
				FPlatformProcess::LaunchURL(recievedUri.GetCharArray().GetData(), NULL, NULL);
#endif
			}
		}

		Status.ExecuteIfBound(needLogin);
	});

	FString url = API_BASE_URL + ENDPOINT_CONNECT;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\"}");
	Request->ProcessRequest();
}

// ---------
// GetClient
// ---------
// GetWalletInfo is used to send a request with 'device_id' as a raw body parameter at http://45.77.189.28:5000/wallet/info to get a response having a 'accounts' which is connected wallet address and 'chainId'.
// The account can be used whenever the user's public address is needed in any transactions.
void UAnkrClient::GetWalletInfo(FAnkrDelegate Result)
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
				for (int32 i = 0; i < accountsObject.Num(); i++)
				{
					accounts.Add(accountsObject[i]->AsString());
				}
				activeAccount = accounts[0];

				chainId = JsonObject->GetIntegerField("chainId");

				updateNFTExample->SetAccount(activeAccount, chainId);
				wearableNFTExample->SetAccount(activeAccount, chainId);
				advertisementManager->SetAccount(activeAccount, chainId);

				data = FString("Active Account: ").Append(activeAccount).Append(" | Chain Id: ").Append(FString::FromInt(chainId));
			}
			else
			{
				data = JsonObject->GetStringField("msg");
			}
		}

		Result.ExecuteIfBound(data);
	});

	FString url = API_BASE_URL + ENDPOINT_WALLET_INFO;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\"}");
	Request->ProcessRequest();
}

// ----------------
// GetActiveAccount
// ----------------
// Returns the currently connected wallet account's public address.
FString UAnkrClient::GetActiveAccount()
{
	if (!activeAccount.IsEmpty())
	{
		return activeAccount;
	}

	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetActiveAccount - Please Connect Wallet on the main screen to get the account address."));
	//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, *FString("AnkrClient - GetActiveAccount - Please Connect Wallet on the main screen to get the account address."));
	return "";
}

// -------
// SendABI
// -------
// SendABI is used to send a request with 'abi' as a raw body parameter at http://45.77.189.28:5000/abi to get a response 'abi' which is the abi hash generated by the backend.
void UAnkrClient::SendABI(FString abi, FAnkrDelegate Result)
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
			Result.ExecuteIfBound(JsonObject->GetStringField("abi"));
		}
	});

	const TCHAR* find = TEXT("\"");
	const TCHAR* replace = TEXT("\\\"");
	FString body = FString("{\"abi\": \"" + abi.Replace(find, replace, ESearchCase::IgnoreCase) + "\"}");

	FString url = API_BASE_URL + ENDPOINT_ABI;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString(body);
	Request->ProcessRequest();
}

// ---------------
// SendTransaction
// ---------------
// SendTransaction is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args' } as a raw body parameter at http://45.77.189.28:5000/send/transaction to get a response having a 'ticket'.
// The session saved during GetClient will be used to open metamask.
// Metamask will show popup to sign or confirm the transaction for that ticket.
void UAnkrClient::SendTransaction(FString contract, FString abi_hash, FString method, FString args, FAnkrTicket Ticket)
{
	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Ticket, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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

#if PLATFORM_ANDROID
			AnkrUtility::SetLastRequest("SendTransaction");
			FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
		}

		Ticket.ExecuteIfBound(data);
	});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, contract, abi_hash, method, args]()
	{
		FString url = API_BASE_URL + ENDPOINT_SEND_TRANSACTION;
		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
		Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
		Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");
		Request->ProcessRequest();
	});
}

// ---------------
// GetTicketResult
// ---------------
// GetTicketResult is used to send a request with { 'device_id', 'ticket' } as a raw body parameter at http://45.77.189.28:5000/result to get a response having a 'code' and 'status'.
// The 'status' shows whether the result for the ticket signed has a success or failure.
// The 'code' shows a code number related to a specific failure or success.
void UAnkrClient::GetTicketResult(FString ticketId, FAnkrTicketResult Result)
{
	int i = 0;
	int code = 0;

	while (i < 10 && code == 0)
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

				Result.ExecuteIfBound("Transaction status: " + status, code);
			}
		});

		FString url = API_BASE_URL + ENDPOINT_RESULT;
		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
		Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
		Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"ticket\": \"" + ticketId + "\" }");
		Request->ProcessRequest();
		FPlatformProcess::Sleep(10000);
		i++;
	}
}

// -------
// GetData
// -------
// GetData is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args' } as a raw body parameter at http://45.77.189.28:5000/wallet/call/method to get a response data from readable functions from the contract.
void UAnkrClient::GetData(FString contract, FString abi_hash, FString method, FString args, FAnkrDelegate Result)
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

		Result.ExecuteIfBound(content);
	});

	FString url = API_BASE_URL + ENDPOINT_CALL_METHOD;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");
	Request->ProcessRequest();
}

// -----------
// SignMessage
// -----------
// SignMessage is used to send a request with { 'device_id', 'message' } as a raw body parameter at http://45.77.189.28:5000/sign/message to get a response having a 'ticket'.
// The session saved during GetClient will be used to open metamask.
// Metamask will show popup to sign or confirm the transaction for that ticket.
void UAnkrClient::SignMessage(FString message, FAnkrDelegate Result)
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

#if PLATFORM_ANDROID
			AnkrUtility::SetLastRequest("SignMessage");
			FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif

			Result.ExecuteIfBound(ticketId);
		}
	});

	FString url = API_BASE_URL + ENDPOINT_SIGN_MESSAGE;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"message\":\"" + message + "\"}");
	Request->ProcessRequest();
}

// ------------
// GetSignature
// ------------
// GetSignature is used to send a request with { 'device_id', 'ticket' } as a raw body parameter at http://45.77.189.28:5000/result to get a response having a 'data' object with 'signature' string field.
// The signature will used in addition to the message for verification.
void UAnkrClient::GetSignature(FString ticket, FAnkrDelegate Result)
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

			Result.ExecuteIfBound(data->GetStringField("signature"));
		}
	});
	
	FString url = API_BASE_URL + ENDPOINT_RESULT;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"ticket\":\"" + ticket + "\"}");
	Request->ProcessRequest();
}

// -------------
// VerifyMessage
// -------------
// VerifyMessage is used to send a request with { 'device_id', 'message', 'signature' } as a raw body parameter at http://45.77.189.28:5000/verify/message to get a response having an account 'address'.
// The account address will the connected wallet's public address.
void UAnkrClient::VerifyMessage(FString message, FString signature, FAnkrDelegate Result)
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
			Result.ExecuteIfBound(JsonObject->GetStringField("address"));
		}
	});

	FString url = API_BASE_URL + ENDPOINT_VERIFY_MESSAGE;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(USER_AGENT_KEY, USER_AGENT_VALUE);
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