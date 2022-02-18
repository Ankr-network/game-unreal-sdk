#include "MirageClient.h"
#include <string>

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#endif

UMirageClient::UMirageClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	baseUrl = "http://45.77.189.28:5000/";
	deviceId = GetDeviceId(); // FGenericPlatformMisc::GetHashedMacAddressString();

	UE_LOG(LogTemp, Warning, TEXT("MirageSDK initialized."));
}

void UMirageClient::HelloWorldFromPlugin()
{
	UE_LOG(LogTemp, Warning, TEXT("UMirageClient::HelloWorldFromPlugin."));
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

				// Output it for debug
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, recievedUri);

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
		FString url = this->baseUrl + "send/transaction";
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

// This function added for get device id on android
// FGenericPlatformMisc::GetHashedMacAddressString() or FGenericPlatformMisc::GetDeviceId() not working
FString UMirageClient::GetDeviceId()
{
#if PLATFORM_ANDROID
	jstring str;
	JNIEnv* env = FAndroidApplication::GetJavaEnv();
	jobject activity = FAndroidApplication::GetGameActivityThis();
	jmethodID mid = env->GetMethodID(env->GetObjectClass(activity),
		"getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
	// try to get the phone ID
	jobject telephony_manager = env->CallObjectMethod(activity, mid,
		env->NewStringUTF("phone"));
	jmethodID mid2 = env->GetMethodID(env->GetObjectClass(telephony_manager),
		"getDeviceId", "()Ljava/lang/String;");
	// It is a phone
	str = (jstring)env->CallObjectMethod(telephony_manager, mid2);

	jsize len = env->GetStringUTFLength(str);

	if (len == 0)
	{
		// try to get the wifi mac address
		jobject wifi_manager = env->CallObjectMethod(activity, mid,
			env->NewStringUTF("wifi"));
		mid = env->GetMethodID(env->GetObjectClass(wifi_manager),
			"getConnectionInfo", "()Landroid/net/wifi/WifiInfo;");
		jobject wifiinfo = env->CallObjectMethod(wifi_manager, mid);
		mid = env->GetMethodID(env->GetObjectClass(wifiinfo),
			"getMacAddress", "()Ljava/lang/String;");
		str = (jstring)env->CallObjectMethod(wifiinfo, mid);
		len = env->GetStringUTFLength(str);
	}

	char* _deviceId = (char*)calloc(len + 1, 1);
	env->GetStringUTFRegion(str, 0, len, _deviceId);
	env->DeleteLocalRef(str);
	FString id(_deviceId);
	free(_deviceId);
	return FMD5::HashAnsiString(*id);
#else
	return FPlatformMisc::GetHashedMacAddressString();
#endif
}