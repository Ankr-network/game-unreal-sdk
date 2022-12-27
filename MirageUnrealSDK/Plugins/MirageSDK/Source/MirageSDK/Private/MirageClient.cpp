#include "MirageClient.h"
#include "MirageSaveGame.h"
#include "MirageUtility.h"
#include "PayloadBuilder.h"

UMirageClient::UMirageClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UMirageClient::Initialize()
{
	if (UMirageSaveGame::Load() == nullptr)
	{
		UMirageSaveGame::Save(FGuid::NewGuid().ToString());
	}
	UMirageSaveGame* load = UMirageSaveGame::Load();

	UMirageUtility::SetDeviceID(load->UniqueId);
	UMirageUtility::SetDevelopment(true);

	UE_LOG(LogTemp, Warning, TEXT("MirageClient - Initialize - MirageSDK will use device id: %s."), *UMirageUtility::GetDeviceID());
}

void UMirageClient::Ping(const FMirageCallCompleteDynamicDelegate& Result)
{
	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_PING, "GET", "", [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - Ping: %s"), *content);
			callback.ExecuteIfBound(content, "", "", -1, false);

		}, Result, false);
}

void UMirageClient::ConnectWallet(const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id", UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID())}
		});
	
	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_CONNECT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - ConnectWallet: %s"), *content);

			bool result = jsonObject->GetBoolField("result");
			if (result)
			{
				bool login      = jsonObject->GetBoolField("login");
				FString session = jsonObject->GetStringField("session");
				FString uri     = jsonObject->GetStringField("uri");
				
				UMirageUtility::SetNeedLogin(login);
				UMirageUtility::SetSession(session);
				UMirageUtility::SetWalletConnectDeepLink(uri);

				if (login)
				{
#if PLATFORM_ANDROID || PLATFORM_IOS
					UMirageUtility::SetLastRequest("ConnectWallet");
					FPlatformProcess::LaunchURL(uri.GetCharArray().GetData(), NULL, NULL);
#endif
				}

				callback.ExecuteIfBound(content, "", "", -1, login);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("MirageClient - ConnectWallet - Couldn't connect, when result is false, see details:\n%s"), *content);
			}

		}, Result, false);
}

void UMirageClient::GetWalletInfo(const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"device_id", UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID())}
		});

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_WALLET_INFO, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - GetWalletInfo: %s"), *content);

			FString data = content;

			bool result = jsonObject->GetBoolField("result");
			if (result)
			{
				TArray<TSharedPtr<FJsonValue>> accounts = jsonObject->GetArrayField("accounts");
				if (accounts.Num() > 0)
				{
					for (int32 i = 0; i < accounts.Num(); i++)
					{
						UMirageUtility::AddWalletAddress(accounts[i]->AsString());
					}

					UMirageUtility::SetChainID(jsonObject->GetIntegerField("chainId"));

					data = FString("Wallet Address: ").Append(UMirageUtility::GetWalletAddress()).Append(" | Chain Id: ").Append(FString::FromInt(UMirageUtility::GetChainID()));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("MirageClient - GetWalletInfo - Couldn't get an account, wallet is not connected: %s"), *content);

					UMirageSaveGame::Erase();
					UMirageUtility::Reset();
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("MirageClient - GetWalletInfo - Couldn't get a valid response: %s"), *content);
				data = jsonObject->GetStringField("msg");
			}

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}

void UMirageClient::SendABI(FString abi, const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"abi", UPayloadBuilder::FStringToJsonValue(abi)}
		});
	
	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_ABI, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - SendABI - response: %s"), *content);

			FString abi = jsonObject->GetStringField("abi");
			
			callback.ExecuteIfBound(content, abi, "", -1, false);
			
		}, Result, false);
}

void UMirageClient::SendTransaction(FString contract, FString abi_hash, FString method, FString args, const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id",		 UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID())},
			{"contract_address", UPayloadBuilder::FStringToJsonValue(contract)},
			{"abi_hash",		 UPayloadBuilder::FStringToJsonValue(abi_hash)},
			{"method",			 UPayloadBuilder::FStringToJsonValue(method)},
			{"args",			 UPayloadBuilder::FStringToJsonValue(args)} 
		});

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - SendTransaction - response: %s"), *content);

			FString ticket = jsonObject->GetStringField("ticket");

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(UMirageUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

			UMirageUtility::SetLastRequest("SendTransaction");
			callback.ExecuteIfBound(content, ticket, "", -1, false);

		}, Result, true);
}

void UMirageClient::GetTicketResult(FString ticketId, const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"ticket", UPayloadBuilder::FStringToJsonValue(ticketId)}
		});

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_RESULT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - GetTicketResult - response: %s"), *content);

			FString status = jsonObject->GetStringField("status");
			int code       = jsonObject->GetIntegerField("code");
			
			callback.ExecuteIfBound(content, status, "", code, false);

		}, Result, false);
}

void UMirageClient::CallMethod(FString contract, FString abi_hash, FString method, FString args, const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id",		 UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID())},
			{"contract_address", UPayloadBuilder::FStringToJsonValue(contract)},
			{"abi_hash",		 UPayloadBuilder::FStringToJsonValue(abi_hash)},
			{"method",			 UPayloadBuilder::FStringToJsonValue(method)},
			{"args",			 UPayloadBuilder::FStringToJsonValue(args)} 
		});
	UE_LOG(LogTemp, Warning, TEXT("MirageClient - CallMethod - payload: %s."), *payload);
	
	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - CallMethod - response: %s"), *content);

			callback.ExecuteIfBound(content, content, "", -1, false);

		}, Result, false);
}

void UMirageClient::SignMessage(FString message, const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id", UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID())},
			{"message",   UPayloadBuilder::FStringToJsonValue(message)} 
		});

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_SIGN_MESSAGE, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - SignMessage - response: %s"), *content);

			FString ticket = jsonObject->GetStringField("ticket");

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(UMirageUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

			UMirageUtility::SetLastRequest("SignMessage");
			callback.ExecuteIfBound(content, ticket, "", -1, false);

		}, Result, true);
}

void UMirageClient::GetSignature(FString ticket, const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"ticket", UPayloadBuilder::FStringToJsonValue(ticket)}
		});

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_RESULT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - GetSignature - response: %s"), *content);

			TSharedPtr<FJsonObject> data = jsonObject->GetObjectField("data");
			FString signature			 = data->GetStringField("signature");

			callback.ExecuteIfBound(content, signature, "", -1, false);

		}, Result, false);
}

void UMirageClient::VerifyMessage(FString message, FString signature, const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id", UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID())},
			{"message",   UPayloadBuilder::FStringToJsonValue(message)},
			{"signature", UPayloadBuilder::FStringToJsonValue(signature)} 
		});

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_VERIFY_MESSAGE, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - VerifyMessage - response: %s"), *content);

			FString address = jsonObject->GetStringField("address");

			callback.ExecuteIfBound(content, address, "", -1, false);

		}, Result, false);
}

void UMirageClient::CollectStatistics(FString _app_id, FString _device_id, FString _public_address)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"app_id",         UPayloadBuilder::FStringToJsonValue(_app_id)},
			{"device_id",      UPayloadBuilder::FStringToJsonValue(_device_id)},
			{"public_address", UPayloadBuilder::FStringToJsonValue(_public_address)} 
		});

	SendRequest(UMirageUtility::GetStatUrl() + ENDPOINT_STATS_COLLECT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - CollectStatistics - response: %s"), *content);
		}, false);
}

FString UMirageClient::GetLastRequest()
{
	return UMirageUtility::GetLastRequest();
}

void UMirageClient::SetLastRequest(FString _lastRequest)
{
	UMirageUtility::SetLastRequest(_lastRequest);
}