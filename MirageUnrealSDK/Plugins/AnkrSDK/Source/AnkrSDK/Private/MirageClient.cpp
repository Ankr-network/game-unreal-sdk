#include "MirageClient.h"
#include "MirageSaveGame.h"
#include <string>
#include "ItemInfo.h"
#include "RequestBodyStructure.h"

// -----------
// Constructor
// -----------
// First of all a deviceId is generated and saved which will be associated with requests that are send to backend http://45.77.189.28:5000/{endpoint}. Secondly updateNFTExample and wearableNFTExample objects are instantiated.
UMirageClient::UMirageClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (UMirageSaveGame::Load() == nullptr)
	{
		UMirageSaveGame::Save(FGuid::NewGuid().ToString());
	}
	UMirageSaveGame* load = UMirageSaveGame::Load();

	deviceId = load->UniqueId;
	baseUrl  = "http://45.77.189.28:5000/";

	UE_LOG(LogTemp, Warning, TEXT("ANKR SDK initialized - deviceId: %s | baseUrl: %s"), *load->UniqueId, *baseUrl);

	if (updateNFTExample == nullptr)
	{
		updateNFTExample = NewObject<UUpdateNFTExample>();
	}
	if (wearableNFTExample == nullptr)
	{
		wearableNFTExample = NewObject<UWearableNFTExample>();
	}
}

// ----
// Ping
// ----
// Ping is a request sent at http://45.77.189.28:5000/ping to check if a response is obtained from server.
void UMirageClient::Ping(FMirageDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - Ping: %s"), *Response->GetContentAsString());
			Result.ExecuteIfBound(*Response->GetContentAsString());
		});

	FString url = baseUrl + "ping";

	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

// ---------
// GetClient
// ---------
// GetClient is used to send a request with 'device_id' as a raw body parameter at http://45.77.189.28:5000/connect to get a response having a 'uri' deeplink to openmetamask, a 'session' and a 'login' information if required.
// The 'uri' deeplink will only work on mobile devices as on desktop a QR Code will be generated at the time the login button is pressed.
// The session is saved to a variable for later use.
bool UMirageClient::GetClient(FMirageConnectionStatus Status)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Status, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - GetClient - GetContentAsString: %s"), *Response->GetContentAsString());
			
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString recievedUri = JsonObject->GetStringField("uri");
				FString sessionId	= JsonObject->GetStringField("session");
				bool needLogin		= JsonObject->GetBoolField("login");
				session				= sessionId;

				updateNFTExample->Init(deviceId, baseUrl, session);
				wearableNFTExample->Init(deviceId, baseUrl, session);

				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, recievedUri);
				UE_LOG(LogTemp, Warning, TEXT("MirageClient - GetClient - uri: %s"), *recievedUri);

				if (needLogin) 
				{
#if PLATFORM_ANDROID
					FPlatformProcess::LaunchURL(recievedUri.GetCharArray().GetData(), NULL, NULL);
#endif
				}
				Status.ExecuteIfBound(true);
			}
			else 
			{
				Status.ExecuteIfBound(false);
			}

		});

	FString url = baseUrl + "connect";
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, url);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\"}");
	Request->ProcessRequest();
	return true;
}

// ---------
// GetClient
// ---------
// GetWalletInfo is used to send a request with 'device_id' as a raw body parameter at http://45.77.189.28:5000/wallet/info to get a response having a 'accounts' which is connected wallet address and 'chainId'.
// The account can be used whenever the user's public address is needed in any transactions.
void UMirageClient::GetWalletInfo(FMirageDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - GetWalletInfo - GetContentAsString: %s"), *Response->GetContentAsString());

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, Response->GetContentAsString());

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

					data = FString("Active Account: ").Append(activeAccount).Append(" | Chain Id: ").Append(FString::FromInt(chainId));
				}
			}

			Result.ExecuteIfBound(data);
		});

	FString url = baseUrl + "wallet/info";
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, url);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\"}");
	Request->ProcessRequest();
}

// ----------------
// GetActiveAccount
// ----------------
// Returns the currently connected wallet account's public address.
FString UMirageClient::GetActiveAccount()
{
	if (!activeAccount.IsEmpty())
	{
		return activeAccount;
	}

	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, *FString("MirageClient - GetActiveAccount - Please press Wallet Info on the main screen to get the account address."));
	return "";
}

// -------
// SendABI
// -------
// SendABI is used to send a request with 'abi' as a raw body parameter at http://45.77.189.28:5000/abi to get a response 'abi' which is the abi hash generated by the backend.
void UMirageClient::SendABI(FString abi, FMirageDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("SendABI Response: %s"), *data);
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				Result.ExecuteIfBound(JsonObject->GetStringField("abi"));
			}
		});

	FString url = baseUrl + "abi";

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	const TCHAR* find = TEXT("\"");
	const TCHAR* replace = TEXT("\\\"");
	FString body = FString("{\"abi\": \"" + abi.Replace(find, replace, ESearchCase::IgnoreCase) + "\"}");
	//UE_LOG(LogTemp, Warning, TEXT("SendABI body: %s"), *body);
	Request->SetContentAsString(*body); // erc20 abi
	Request->ProcessRequest();
}

// ---------------
// SendTransaction
// ---------------
// SendTransaction is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args' } as a raw body parameter at http://45.77.189.28:5000/send/transaction to get a response having a 'ticket'.
// The session saved during GetClient will be used to open metamask.
// Metamask will show popup to sign or confirm the transaction for that ticket.
void UMirageClient::SendTransaction(FString contract, FString abi_hash, FString method, FString args, FMirageTicket Ticket)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Ticket, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("SendTransaction Response: %s"), *data);

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString ticketId = JsonObject->GetStringField("ticket");
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, ticketId);
				data = ticketId;
			}

			Ticket.ExecuteIfBound(data);
		});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, contract, abi_hash, method, args]()
		{
			FString url = baseUrl + "send/transaction";

			Request->SetURL(url);
			Request->SetVerb("POST");
			Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
			Request->SetHeader("Content-Type", TEXT("application/json"));
			Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}"); // erc20 abi
			Request->ProcessRequest();
		});

#if PLATFORM_ANDROID
	FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
}

// ---------------
// GetTicketResult
// ---------------
// GetTicketResult is used to send a request with { 'device_id', 'ticket' } as a raw body parameter at http://45.77.189.28:5000/result to get a response having a 'code' and 'status'.
// The 'status' shows whether the result for the ticket signed has a success or failure.
// The 'code' shows a code number related to a specific failure or success.
void UMirageClient::GetTicketResult(FString ticketId, FMirageTicketResult Result)
{
	int i = 0;
	int code = 0;

	while (i < 10 && code == 0)
	{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
		Request->OnProcessRequestComplete().BindLambda([Result, ticketId, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
			{
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

				if (FJsonSerializer::Deserialize(Reader, JsonObject))
				{
					int code = JsonObject->GetIntegerField("code");
					FString status = JsonObject->GetStringField("status");
					Result.ExecuteIfBound("Transaction status: " + status, code);
				}
			});

		FString url = baseUrl + "result";

		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"ticket\": \"" + ticketId + "\" }"); // erc20 abi
		Request->ProcessRequest();
		FPlatformProcess::Sleep(10000);
		i++;
	}
}

// -------
// GetData
// -------
// GetData is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args' } as a raw body parameter at http://45.77.189.28:5000/wallet/call/method to get a response data from readable functions from the contract.
void UMirageClient::GetData(FString contract, FString abi_hash, FString method, FString args, FMirageDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			Result.ExecuteIfBound(Response->GetContentAsString());
		});

	FString url = baseUrl + "call/method";

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}"); // erc20 abi
	Request->ProcessRequest();
}

// -----------
// SignMessage
// -----------
// SignMessage is used to send a request with { 'device_id', 'message' } as a raw body parameter at http://45.77.189.28:5000/sign/message to get a response having a 'ticket'.
// The session saved during GetClient will be used to open metamask.
// Metamask will show popup to sign or confirm the transaction for that ticket.
void UMirageClient::SignMessage(FString message, FMirageDelegate Result)
{
	http = &FHttpModule::Get();
	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString ticketId = JsonObject->GetStringField("ticket");
				UE_LOG(LogTemp, Warning, TEXT("ticket: %s"), *ticketId);

#if PLATFORM_ANDROID
				FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
				Result.ExecuteIfBound(ticketId);
			}
		});
	
	FString url = baseUrl + "sign/message";

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"message\":\"" + message + "\"}"); // erc20 abi
	Request->ProcessRequest();
}

// ------------
// GetSignature
// ------------
// GetSignature is used to send a request with { 'device_id', 'ticket' } as a raw body parameter at http://45.77.189.28:5000/result to get a response having a 'data' object with 'signature' string field.
// The signature will used in addition to the message for verification.
void UMirageClient::GetSignature(FString ticket, FMirageDelegate Result)
{
	http = &FHttpModule::Get();
	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				TSharedPtr<FJsonObject> data = JsonObject->GetObjectField("data");
				UE_LOG(LogTemp, Warning, TEXT("signature: %s"), *data->GetStringField("signature"));
				Result.ExecuteIfBound(data->GetStringField("signature"));
			}
		});
	
	FString url = baseUrl + "result";

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"ticket\":\"" + ticket + "\"}"); // erc20 abi
	Request->ProcessRequest();
}

// -------------
// VerifyMessage
// -------------
// VerifyMessage is used to send a request with { 'device_id', 'message', 'signature' } as a raw body parameter at http://45.77.189.28:5000/verify/message to get a response having an account 'address'.
// The account address will the connected wallet's public address.
void UMirageClient::VerifyMessage(FString message, FString signature, FMirageDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				UE_LOG(LogTemp, Warning, TEXT("address: %s"), *JsonObject->GetStringField("address"));
				Result.ExecuteIfBound(JsonObject->GetStringField("address"));
			}
		});

	FString url = baseUrl + "verify/message";
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"message\":\"" + message + "\", \"signature\":\"" + signature + "\"}"); // erc20 abi
	Request->ProcessRequest();
}