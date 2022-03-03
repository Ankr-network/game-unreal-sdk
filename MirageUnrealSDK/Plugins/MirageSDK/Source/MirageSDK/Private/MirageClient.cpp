#include "MirageClient.h"
#include "MirageSaveGame.h"
#include <string>

UMirageClient::UMirageClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (UMirageSaveGame::Load() == nullptr)
	{
		UMirageSaveGame::Save(FGuid::NewGuid().ToString());
	}
	UMirageSaveGame* load = UMirageSaveGame::Load();

	deviceId = load->UniqueId;
	baseUrl = "http://45.77.189.28:5000/";

	UE_LOG(LogTemp, Warning, TEXT("MirageSDK initialized - UniqueId: %s | baseUrl: %s"), *load->UniqueId, *baseUrl);
}

bool UMirageClient::GetClient(FMirageConnectionStatus Status)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Status, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			// Deserialize the json data given Reader and the actual object to deserialize
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString recievedUri = JsonObject->GetStringField("uri");
				FString sessionId = JsonObject->GetStringField("session");
				bool needLogin = JsonObject->GetBoolField("login");
				session = sessionId;

				// Output it for debug
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, recievedUri);
				UE_LOG(LogTemp, Warning, TEXT("uri: %s"), *recievedUri);

				// Set session from backend clientId for future calls
				this->clientId = sessionId;

				// Open Metamask
				if (needLogin) {
					FPlatformProcess::LaunchURL(recievedUri.GetCharArray().GetData(), NULL, NULL);
				}
				Status.Execute(true);
			}
			else {
				Status.Execute(false);
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

void UMirageClient::SendTransaction(FString contract, FString abi, FString method, FString args, FMirageTicket Ticket)
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

				Ticket.Execute(ticketId);
			}
		});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, contract, abi, method, args]() {
		FString url = baseUrl + "send/transaction";
		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->SetContentAsString("{\"device_id\": \"" + this->deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}"); // erc20 abi
		Request->ProcessRequest();
		});

	// Open Metamask
	FPlatformProcess::LaunchURL(this->clientId.GetCharArray().GetData(), NULL, NULL);
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
					Result.Execute("Transaction status: " + status, code);
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

void UMirageClient::GetData(FString contract, FString abi, FString method, FString args, FMirageDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			Result.Execute(Response->GetContentAsString());
		});

	FString url = baseUrl + "call/method";
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	// Send clientId to backend to redirect metamask
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}"); // erc20 abi
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
			Result.Execute(JsonObject->GetStringField("abi_hash"));
		});

	FString url = baseUrl + "call/method";
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	// Send clientId to backend to redirect metamask

	const TCHAR* find = TEXT("\"");
	const TCHAR* replace = TEXT("\\\"");
	Request->SetContentAsString("{\"abi\": \"" + abi.Replace(find, replace, ESearchCase::IgnoreCase) + "\"}"); // erc20 abi
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
				FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
				Result.ExecuteIfBound(JsonObject->GetStringField("ticket"));
			}
		});
	
	FString url = baseUrl + "sign/message";
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	// Send clientId to backend to redirect metamask
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
	// Send clientId to backend to redirect metamask
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
	// Send clientId to backend to redirect metamask
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"message\":\"" + message + "\", \"signature\":\"" + signature + "\"}"); // erc20 abi
	Request->ProcessRequest();
}