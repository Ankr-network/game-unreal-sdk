#include "AnkrClient.h"
#include "AnkrSaveGame.h"
#include "AnkrUtility.h"

#if PLATFORM_WINDOWS
#include "Windows/LibraryManager.h"
#elif PLATFORM_MAC
#include "Mac/LibraryManager.h"
#elif PLATFORM_IOS
#include "iOS/LibraryManager.h"
#elif PLATFORM_ANDROID
#include "Android/LibraryManager.h"
#endif

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

#if PLATFORM_WINDOWS
	LibraryManager::GetInstance().Load();
	LibraryManager::GetInstance().InitializeFunction(false, TCHAR_TO_UTF8(*deviceId), [](const char* _message) { UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(_message)); });
#elif PLATFORM_MAC
	LibraryManager::GetInstance().Load();
	LibraryManager::GetInstance().Initialize(false, deviceId);
#elif PLATFORM_IOS
	LibraryManager::GetInstance().Load();
	LibraryManager::GetInstance().Inialize(false, deviceId);
#elif PLATFORM_ANDROID
	LibraryManager::GetInstance().Load();
	LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(JCM_Initialize, false, GetJString(deviceId));
#endif
}

// Ping is to make sure if we can ping the Ankr API.
void UAnkrClient::Ping(const FAnkrCallCompleteDynamicDelegate& Result)
{
	if (LibraryManager::GetInstance().AddCall("Ping", Result))
	{
#if PLATFORM_WINDOWS

		LibraryManager::GetInstance().PingFunction([](bool _success, const char* _data)
			{
				LibraryManager::GetInstance().FlushCall("Ping", _success, _data);
			});

#elif PLATFORM_MAC

		LibraryManager::GetInstance().Ping();

#elif PLATFORM_IOS

		LibraryManager::GetInstance().Ping();

#elif PLATFORM_ANDROID

		LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(LibraryManager::GetInstance().JCM_Ping);

#endif
		}
}

// ConnectWallet is used to connect wallet (Metamask). 
// Wallet app will be opened on mobile devices only, as on desktop (Windows/Mac) a QR Code will be generated at the time the login button is pressed. Scan the QR Code with your Wallet app from mobile.
void UAnkrClient::ConnectWallet(const FAnkrCallCompleteDynamicDelegate& Result)
{
	FString body = FString("{\"device_id\":\"" + deviceId + "\"}");

	if (LibraryManager::GetInstance().AddCall("ConnectWallet", Result))
	{
#if PLATFORM_WINDOWS
	
		LibraryManager::GetInstance().ConnectWalletFunction(TCHAR_TO_UTF8(*body), [](bool _success, const char* _data)
			{
				LibraryManager::GetInstance().FlushCall("ConnectWallet", _success, _data);
			});
	
#elif PLATFORM_MAC

	LibraryManager::GetInstance().ConnectWallet(body);

#elif PLATFORM_IOS

	LibraryManager::GetInstance().ConnectWallet(body);


#elif PLATFORM_ANDROID
	
		LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(LibraryManager::GetInstance().JCM_ConnectWallet, LibraryManager::GetInstance().GetJString(body));
	
#endif
	}
}

// GetWalletInfo is used to get the connected wallet account and the chainId.
// The account can be used whenever the user's public address is needed in any transactions.
void UAnkrClient::GetWalletInfo(const FAnkrCallCompleteDynamicDelegate& Result)
{
	FString body = FString("{\"device_id\":\"" + deviceId + "\"}");

	if (LibraryManager::GetInstance().AddCall("GetWallet", Result))
	{
#if PLATFORM_WINDOWS
	
		LibraryManager::GetInstance().GetWalletFunction(TCHAR_TO_UTF8(*body), [](bool _success, const char* _data)
			{
				LibraryManager::GetInstance().FlushCall("GetWallet", _success, _data);
			});

#elif PLATFORM_MAC

		LibraryManager::GetInstance().GetWallet(body);

#elif PLATFORM_IOS

		LibraryManager::GetInstance().GetWallet(body);
	
#elif PLATFORM_ANDROID

		LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(LibraryManager::GetInstance().JCM_GetWallet, LibraryManager::GetInstance().GetJString(body));
#endif
	}
}

// Returns the currently connected wallet address.
FString UAnkrClient::GetActiveAccount()
{
	return !activeAccount.IsEmpty() ? activeAccount : "";
}

// SendABI is used to get the abi hash.
void UAnkrClient::SendABI(FString abi, const FAnkrCallCompleteDynamicDelegate& Result)
{
	const TCHAR* find = TEXT("\"");
	const TCHAR* replace = TEXT("\\\"");
	FString body = FString("{\"abi\": \"" + abi.Replace(find, replace, ESearchCase::IgnoreCase) + "\"}");

	if (LibraryManager::GetInstance().AddCall("SendABI", Result))
	{
#if PLATFORM_WINDOWS

		LibraryManager::GetInstance().SendABIFunction(TCHAR_TO_UTF8(*body), [](bool _success, const char* _data)
			{
				LibraryManager::GetInstance().FlushCall("SendABI", _success, _data);
			});

#elif PLATFORM_MAC

		LibraryManager::GetInstance().SendABI(body);

#elif PLATFORM_IOS

		LibraryManager::GetInstance().SendABI(body);

#elif PLATFORM_ANDROID

		LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(LibraryManager::GetInstance().JCM_SendABI, LibraryManager::GetInstance().GetJString(body));
#endif
	}
}

// SendTransaction is used to send a trasaction provided that the paramters are entered correctly.
void UAnkrClient::SendTransaction(FString contract, FString abi_hash, FString method, FString args, const FAnkrCallCompleteDynamicDelegate& Result)
{
	FString body = FString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");

	if (LibraryManager::GetInstance().AddCall("SendTransaction", Result))
	{
#if PLATFORM_WINDOWS

		LibraryManager::GetInstance().SendTransactionFunction(TCHAR_TO_UTF8(*body), [](bool _success, const char* _data)
			{
				LibraryManager::GetInstance().FlushCall("SendTransaction", _success, _data);
			});

#elif PLATFORM_MAC

		LibraryManager::GetInstance().SendTransaction(body);

#elif PLATFORM_IOS

		LibraryManager::GetInstance().SendTransaction(body);

#elif PLATFORM_ANDROID

		LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(LibraryManager::GetInstance().JCM_SendTransaction, LibraryManager::GetInstance().GetJString(body));
#endif
	}
}

// GetTicketResult is used to get the status of the ticket having a 'code' and 'status'.
// The 'status' shows whether the result for the ticket signed has a success or failure.
// The 'code' shows a code number related to a specific failure or success.
void UAnkrClient::GetTicketResult(FString ticketId, const FAnkrCallCompleteDynamicDelegate& Result)
{
	FString body = FString("{\"device_id\": \"" + deviceId + "\", \"ticket\": \"" + ticketId + "\" }");

	if (LibraryManager::GetInstance().AddCall("GetTicketResult", Result))
	{
#if PLATFORM_WINDOWS

		LibraryManager::GetInstance().GetResultFunction(TCHAR_TO_UTF8(*body), [](bool _success, const char* _data)
			{
				LibraryManager::GetInstance().FlushCall("GetTicketResult", _success, _data);
			});

#elif PLATFORM_MAC

		LibraryManager::GetInstance().GetResult(body);

#elif PLATFORM_IOS

		LibraryManager::GetInstance().GetResult(body);

#elif PLATFORM_ANDROID
		
			LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(LibraryManager::GetInstance().JCM_GetResult, LibraryManager::GetInstance().GetJString(body));
#endif
	}
}

// GetData is used to get data from readable functions from the contract provided that the parameters are entered correctly.
void UAnkrClient::GetData(FString contract, FString abi_hash, FString method, FString args, const FAnkrCallCompleteDynamicDelegate& Result)
{
	FString body = FString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");

	if (LibraryManager::GetInstance().AddCall("CallMethod", Result))
	{
#if PLATFORM_WINDOWS

		LibraryManager::GetInstance().CallMethodFunction(TCHAR_TO_UTF8(*body), [](bool _success, const char* _data)
			{
				LibraryManager::GetInstance().FlushCall("CallMethod", _success, _data);
			});

#elif PLATFORM_MAC

		LibraryManager::GetInstance().CallMethod(body);

#elif PLATFORM_IOS

		LibraryManager::GetInstance().CallMethod(body);

#elif PLATFORM_ANDROID
	
			LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(LibraryManager::GetInstance().JCM_CallMethod, LibraryManager::GetInstance().GetJString(body));
#endif
	}
}

// SignMessage is used to to sign and message, the ticket will be generated.
// Metamask will show popup to sign or confirm the transaction for that ticket.
void UAnkrClient::SignMessage(FString message, const FAnkrCallCompleteDynamicDelegate & Result)
{
	FString body = FString("{\"device_id\": \"" + deviceId + "\", \"message\":\"" + message + "\"}");
	if (LibraryManager::GetInstance().AddCall("SignMessage", Result))
	{
#if PLATFORM_WINDOWS

		LibraryManager::GetInstance().SignMessageFunction(TCHAR_TO_UTF8(*body), [](bool _success, const char* _data)
			{
				LibraryManager::GetInstance().FlushCall("SignMessage", _success, _data);
			});

#elif PLATFORM_MAC

		LibraryManager::GetInstance().SignMessage(body);

#elif PLATFORM_IOS

		LibraryManager::GetInstance().SignMessage(body);

#elif PLATFORM_ANDROID

		LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(LibraryManager::GetInstance().JCM_SignMessage, LibraryManager::GetInstance().GetJString(body));
#endif
	}
}

// GetSignature is used to get the result of the signed message ticket and a 'data' object with 'signature' string field will be received.
void UAnkrClient::GetSignature(FString ticket, const FAnkrCallCompleteDynamicDelegate& Result)
{
	FString body = FString("{\"device_id\": \"" + deviceId + "\", \"ticket\":\"" + ticket + "\"}");
	if (LibraryManager::GetInstance().AddCall("GetSignature", Result))
	{
#if PLATFORM_WINDOWS

		LibraryManager::GetInstance().GetResultFunction(TCHAR_TO_UTF8(*body), [](bool _success, const char* _data)
			{
				LibraryManager::GetInstance().FlushCall("GetSignature", _success, _data);
			});

#elif PLATFORM_MAC

		LibraryManager::GetInstance().GetResult(body);

#elif PLATFORM_IOS

		LibraryManager::GetInstance().GetResult(body);

#elif PLATFORM_ANDROID
		if (LibraryManager::GetInstance().AddCall("GetResult", Result))
		{
			LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(LibraryManager::GetInstance().JCM_GetResult, LibraryManager::GetInstance().GetJString(body));
		}
#endif
	}
}

// VerifyMessage is used to confirm whether the user signed the message, an account 'address' will be received.
// The account address will be the connected wallet address.
void UAnkrClient::VerifyMessage(FString message, FString signature, const FAnkrCallCompleteDynamicDelegate& Result)
{
	FString body = FString("{\"device_id\": \"" + deviceId + "\", \"message\":\"" + message + "\", \"signature\":\"" + signature + "\"}");
	if (LibraryManager::GetInstance().AddCall("VerifyMessage", Result))
	{
#if PLATFORM_WINDOWS

		LibraryManager::GetInstance().VerifyMessageFunction(TCHAR_TO_UTF8(*body), [](bool _success, const char* _data)
			{
				LibraryManager::GetInstance().FlushCall("VerifyMessage", _success, _data);
			});

#elif PLATFORM_MAC

		LibraryManager::GetInstance().VerifyMessage(body);

#elif PLATFORM_IOS

		LibraryManager::GetInstance().VerifyMessage(body);

#elif PLATFORM_ANDROID

			LibraryManager::GetInstance().JCO_AnkrClient->CallMethod<void>(LibraryManager::GetInstance().JCM_VerifyMessage, LibraryManager::GetInstance().GetJString(body));
#endif
	}
}

void UAnkrClient::HandlePing(FAnkrCallStruct call)
{
	const FString content = call.data;
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - HandleConnectWallet - %s - Content: %s"), *call.sender, *content);
	call.CallComplete.ExecuteIfBound(true, true, false, "", "", "", -1, call.data);
}
void UAnkrClient::HandleConnectWallet(FAnkrCallStruct call)
{
	const FString content = call.data;
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - HandleConnectWallet - %s - Content: %s"), *call.sender, *content);

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
#if PLATFORM_ANDROID
				AnkrUtility::SetLastRequest("ConnectWallet");
				//FPlatformProcess::LaunchURL(recievedUri.GetCharArray().GetData(), NULL, NULL);
#endif
			}

			call.CallComplete.ExecuteIfBound(call.success, result, needLogin, "", "", walletConnectDeeplink, -1, call.data);
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

}
void UAnkrClient::HandleGetWallet(FAnkrCallStruct call)
{
	const FString content = call.data;
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - HandleGetWallet - %s - Content: %s"), *call.sender, *content);

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

		call.CallComplete.ExecuteIfBound(call.success, result, !activeAccount.IsEmpty(), !activeAccount.IsEmpty() ? activeAccount : "", "", "", - 1, data);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AnkrClient - GetWalletInfo - Couldn't get a valid response:\n%s"), *content);
	}
}
void UAnkrClient::HandleSendABI(FAnkrCallStruct call)
{
	const FString content = call.data;
	//UE_LOG(LogTemp, Warning, TEXT("AnkrClient - HandleGetWallet - %s - Content: %s"), *call.sender, *content);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

	FString data = content;
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		call.CallComplete.ExecuteIfBound(call.success, true, !activeAccount.IsEmpty(), !activeAccount.IsEmpty() ? activeAccount : "", "", "", -1, JsonObject->GetStringField("abi"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AnkrClient - SendABI - Couldn't get a valid response:\n%s"), *content);
	}
}
void UAnkrClient::HandleSendTransaction(FAnkrCallStruct call)
{
	const FString content = call.data;
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - HandleGetWallet - %s - Content: %s"), *call.sender, *content);

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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AnkrClient - SendTransaction - Couldn't get a valid response:\n%s"), *content);
	}

	call.CallComplete.ExecuteIfBound(call.success, true, !activeAccount.IsEmpty(), !activeAccount.IsEmpty() ? activeAccount : "", JsonObject->GetStringField("ticket"), "", -1, data);
}
void UAnkrClient::HandleGetTicketResult(FAnkrCallStruct call)
{
	const FString content = call.data;
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - HandleGetWallet - %s - Content: %s"), *call.sender, *content);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);
}
void UAnkrClient::HandleCallMethod(FAnkrCallStruct call)
{
	const FString content = call.data;
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - HandleGetWallet - %s - Content: %s"), *call.sender, *content);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

	call.CallComplete.ExecuteIfBound(call.success, true, !activeAccount.IsEmpty(), !activeAccount.IsEmpty() ? activeAccount : "", "", "", -1, content);
}
void UAnkrClient::HandleSignMessage(FAnkrCallStruct call)
{
	const FString content = call.data;
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - HandleGetWallet - %s - Content: %s"), *call.sender, *content);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString ticketId = JsonObject->GetStringField("ticket");

#if PLATFORM_ANDROID
		AnkrUtility::SetLastRequest("SignMessage");
		FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif

		call.CallComplete.ExecuteIfBound(call.success, true, !activeAccount.IsEmpty(), !activeAccount.IsEmpty() ? activeAccount : "", ticketId, "", -1, content);
	}
}
void UAnkrClient::HandleGetSignature(FAnkrCallStruct call)
{
	const FString content = call.data;
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - HandleGetWallet - %s - Content: %s"), *call.sender, *content);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		TSharedPtr<FJsonObject> data = JsonObject->GetObjectField("data");

		call.CallComplete.ExecuteIfBound(call.success, true, !activeAccount.IsEmpty(), !activeAccount.IsEmpty() ? activeAccount : "", "", "", -1, data->GetStringField("signature"));
	}
}
void UAnkrClient::HandleVerifyMessage(FAnkrCallStruct call)
{
	const FString content = call.data;
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - HandleGetWallet - %s - Content: %s"), *call.sender, *content);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		call.CallComplete.ExecuteIfBound(call.success, true, !activeAccount.IsEmpty(), !activeAccount.IsEmpty() ? activeAccount : "", "", "", -1, JsonObject->GetStringField("address"));
	}
}

FString UAnkrClient::GetLastRequest()
{
	return AnkrUtility::GetLastRequest();
}

void UAnkrClient::SetLastRequest(FString _lastRequest)
{
	AnkrUtility::SetLastRequest(_lastRequest);
}

void UAnkrClient::Tick(float DeltaTime)
{
	if (LibraryManager::GetInstance().CallQueue.size() > 0)
	{
		FAnkrCallStruct call = LibraryManager::GetInstance().CallQueue.front();
		LibraryManager::GetInstance().CallQueue.pop();

		if (call.sender.Equals(FString("Ping")))
		{
			HandlePing(call);
		}
		else if (call.sender.Equals(FString("ConnectWallet")))
		{
			HandleConnectWallet(call);
		}
		else if (call.sender.Equals(FString("GetWallet")))
		{
			HandleGetWallet(call);
		}
		else if (call.sender.Equals(FString("SendABI")))
		{
			HandleSendABI(call);
		}
		else if (call.sender.Equals(FString("SendTransaction")))
		{
			HandleSendTransaction(call);
		}
		else if (call.sender.Equals(FString("GetTicketResult")))
		{
			HandleGetTicketResult(call);
		}
		else if (call.sender.Equals(FString("CallMethod")))
		{
			HandleCallMethod(call);
		}
		else if (call.sender.Equals(FString("SignMessage")))
		{
			HandleSignMessage(call);
		}
		else if (call.sender.Equals(FString("GetSignature")))
		{
			HandleGetSignature(call);
		}
		else if (call.sender.Equals(FString("VerifyMessage")))
		{
			HandleVerifyMessage(call);
		}
	}
}

bool UAnkrClient::IsTickable() const
{
	return true;
}

bool UAnkrClient::IsTickableInEditor() const
{
	return false;
}

bool UAnkrClient::IsTickableWhenPaused() const
{
	return false;
}

TStatId UAnkrClient::GetStatId() const
{
	return TStatId();
}

UWorld* UAnkrClient::GetWorld() const
{
	return GetOuter()->GetWorld();
}
