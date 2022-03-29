#include "MirageClient.h"
#include "MirageSaveGame.h"
#include <string>
#include "ItemInfo.h"
#include "RequestBodyStructure.h"

UMirageClient::UMirageClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (UMirageSaveGame::Load() == nullptr)
	{
		UMirageSaveGame::Save(FGuid::NewGuid().ToString());
	}
	UMirageSaveGame* load = UMirageSaveGame::Load();

	deviceId = load->UniqueId;
	baseUrl  = "http://45.77.189.28:5000/";

	UE_LOG(LogTemp, Warning, TEXT("MirageSDK initialized - UniqueId: %s | baseUrl: %s"), *load->UniqueId, *baseUrl);

	if (updateNFTExample == nullptr)
	{
		updateNFTExample = NewObject<UUpdateNFTExample>();
	}
	if (wearableNFTExample == nullptr)
	{
		wearableNFTExample = NewObject<UWearableNFTExample>();
	}
}

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
				clientId			= sessionId;

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

void UMirageClient::GetWalletInfo(FMirageDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - GetWalletInfo - GetContentAsString: %s"), *Response->GetContentAsString());

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, Response->GetContentAsString());
				UE_LOG(LogTemp, Warning, TEXT("MirageClient - GetWalletInfo - GetContentAsString: "), *Response->GetContentAsString());

				/*TArray<TSharedPtr<FJsonObject>> accountsObject = JsonObject->GetArrayField("accounts");
				for (int32 i = 0; i < accountsObject.Num(); i++)
				{
					accounts.Add(accountsObject->AsString());
				}
				
				chainId  = JsonObject->GetStringField("chainId");*/

				Result.ExecuteIfBound(Response->GetContentAsString());
			}
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

void UMirageClient::SendABI(FString abi, FMirageDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			UE_LOG(LogTemp, Warning, TEXT("SendABI Response: %s"), *Response->GetContentAsString());
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
	UE_LOG(LogTemp, Warning, TEXT("SendABI body: %s"), *body);
	Request->SetContentAsString(*body); // erc20 abi
	Request->ProcessRequest();
}

void UMirageClient::SendTransaction(FString contract, FString abi_hash, FString method, FString args, FMirageTicket Ticket)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Ticket, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString ticketId = JsonObject->GetStringField("ticket");
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, ticketId);

				Ticket.ExecuteIfBound(ticketId);
			}
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
	FPlatformProcess::LaunchURL(clientId.GetCharArray().GetData(), NULL, NULL);
#endif
}

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
		// Send clientId to backend to redirect metamask
		Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"ticket\": \"" + ticketId + "\" }"); // erc20 abi
		Request->ProcessRequest();
		FPlatformProcess::Sleep(10000);
		i++;
	}
}

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