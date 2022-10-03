#include "AnkrClient.h"
#include "AnkrSaveGame.h"
#include "AnkrUtility.h"
#include "PayloadBuilder.h"

UAnkrClient::UAnkrClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UAnkrClient::Initialize()
{
	if (UAnkrSaveGame::Load() == nullptr)
	{
		UAnkrSaveGame::Save(FGuid::NewGuid().ToString());
	}
	UAnkrSaveGame* load = UAnkrSaveGame::Load();

	UAnkrUtility::SetDeviceID(load->UniqueId);
	UAnkrUtility::SetDevelopment(false);

	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - Initialize - AnkrSDK will use device id: %s."), *UAnkrUtility::GetDeviceID());
}

void UAnkrClient::Ping(const FAnkrCallCompleteDynamicDelegate& Result)
{
	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_PING, "GET", "", [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - Ping: %s"), *content);
			callback.ExecuteIfBound(content, "", "", -1, false);

		}, Result, false);
}

void UAnkrClient::ConnectWallet(const FAnkrCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id", UPayloadBuilder::FStringToJsonValue(UAnkrUtility::GetDeviceID())}
		});
	
	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CONNECT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - ConnectWallet: %s"), *content);

			bool result = jsonObject->GetBoolField("result");
			if (result)
			{
				bool login      = jsonObject->GetBoolField("login");
				FString session = jsonObject->GetStringField("session");
				FString uri     = jsonObject->GetStringField("uri");
				
				UAnkrUtility::SetNeedLogin(login);
				UAnkrUtility::SetSession(session);
				UAnkrUtility::SetWalletConnectDeepLink(uri);

				if (login)
				{
#if PLATFORM_ANDROID || PLATFORM_IOS
					UAnkrUtility::SetLastRequest("ConnectWallet");
					FPlatformProcess::LaunchURL(uri.GetCharArray().GetData(), NULL, NULL);
#endif
				}

				callback.ExecuteIfBound(content, "", "", -1, login);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AnkrClient - ConnectWallet - Couldn't connect, when result is false, see details:\n%s"), *content);
			}

		}, Result, false);
}

void UAnkrClient::GetWalletInfo(const FAnkrCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"device_id", UPayloadBuilder::FStringToJsonValue(UAnkrUtility::GetDeviceID())}
		});

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_WALLET_INFO, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetWalletInfo: %s"), *content);

			FString data = content;

			bool result = jsonObject->GetBoolField("result");
			if (result)
			{
				TArray<TSharedPtr<FJsonValue>> accounts = jsonObject->GetArrayField("accounts");
				if (accounts.Num() > 0)
				{
					for (int32 i = 0; i < accounts.Num(); i++)
					{
						UAnkrUtility::AddWalletAddress(accounts[i]->AsString());
					}

					UAnkrUtility::SetChainID(jsonObject->GetIntegerField("chainId"));

					data = FString("Wallet Address: ").Append(UAnkrUtility::GetWalletAddress()).Append(" | Chain Id: ").Append(FString::FromInt(UAnkrUtility::GetChainID()));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("AnkrClient - GetWalletInfo - Couldn't get an account, wallet is not connected: %s"), *content);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AnkrClient - GetWalletInfo - Couldn't get a valid response: %s"), *content);
				data = jsonObject->GetStringField("msg");
			}

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}

void UAnkrClient::SendABI(FString abi, const FAnkrCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"abi", UPayloadBuilder::FStringToJsonValue(abi)}
		});
	
	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_ABI, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - SendABI - response: %s"), *content);

			FString abi = jsonObject->GetStringField("abi");
			
			callback.ExecuteIfBound(content, abi, "", -1, false);
			
		}, Result, false);
}

void UAnkrClient::SendTransaction(FString contract, FString abi_hash, FString method, FString args, const FAnkrCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id",		 UPayloadBuilder::FStringToJsonValue(UAnkrUtility::GetDeviceID())},
			{"contract_address", UPayloadBuilder::FStringToJsonValue(contract)},
			{"abi_hash",		 UPayloadBuilder::FStringToJsonValue(abi_hash)},
			{"method",			 UPayloadBuilder::FStringToJsonValue(method)},
			{"args",			 UPayloadBuilder::FStringToJsonValue(args)} 
		});

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - SendTransaction - response: %s"), *content);

			FString ticket = jsonObject->GetStringField("ticket");

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(UAnkrUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

			UAnkrUtility::SetLastRequest("SendTransaction");
			callback.ExecuteIfBound(content, ticket, "", -1, false);

		}, Result, true);
}

void UAnkrClient::GetTicketResult(FString ticketId, const FAnkrCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"ticket", UPayloadBuilder::FStringToJsonValue(ticketId)}
		});

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_RESULT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetTicketResult - response: %s"), *content);

			FString status = jsonObject->GetStringField("status");
			int code       = jsonObject->GetIntegerField("code");
			
			callback.ExecuteIfBound(content, status, "", code, false);

		}, Result, false);
}

void UAnkrClient::CallMethod(FString contract, FString abi_hash, FString method, FString args, const FAnkrCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id",		 UPayloadBuilder::FStringToJsonValue(UAnkrUtility::GetDeviceID())},
			{"contract_address", UPayloadBuilder::FStringToJsonValue(contract)},
			{"abi_hash",		 UPayloadBuilder::FStringToJsonValue(abi_hash)},
			{"method",			 UPayloadBuilder::FStringToJsonValue(method)},
			{"args",			 UPayloadBuilder::FStringToJsonValue(args)} 
		});
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - CallMethod - payload: %s."), *payload);
	
	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - CallMethod - response: %s"), *content);

			callback.ExecuteIfBound(content, content, "", -1, false);

		}, Result, false);
}

void UAnkrClient::SignMessage(FString message, const FAnkrCallCompleteDynamicDelegate & Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id", UPayloadBuilder::FStringToJsonValue(UAnkrUtility::GetDeviceID())},
			{"message",   UPayloadBuilder::FStringToJsonValue(message)} 
		});

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_SIGN_MESSAGE, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - SignMessage - response: %s"), *content);

			FString ticket = jsonObject->GetStringField("ticket");

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(UAnkrUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

			UAnkrUtility::SetLastRequest("SignMessage");
			callback.ExecuteIfBound(content, ticket, "", -1, false);

		}, Result, true);
}

void UAnkrClient::GetSignature(FString ticket, const FAnkrCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"ticket", UPayloadBuilder::FStringToJsonValue(ticket)}
		});

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_RESULT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetSignature - response: %s"), *content);

			TSharedPtr<FJsonObject> data = jsonObject->GetObjectField("data");
			FString signature			 = data->GetStringField("signature");

			callback.ExecuteIfBound(content, signature, "", -1, false);

		}, Result, false);
}

void UAnkrClient::VerifyMessage(FString message, FString signature, const FAnkrCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id", UPayloadBuilder::FStringToJsonValue(UAnkrUtility::GetDeviceID())},
			{"message",   UPayloadBuilder::FStringToJsonValue(message)},
			{"signature", UPayloadBuilder::FStringToJsonValue(signature)} 
		});

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_VERIFY_MESSAGE, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - VerifyMessage - response: %s"), *content);

			FString address = jsonObject->GetStringField("address");

			callback.ExecuteIfBound(content, address, "", -1, false);

		}, Result, false);
}

void UAnkrClient::CollectStatistics(FString _app_id, FString _device_id, FString _public_address)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"app_id",         UPayloadBuilder::FStringToJsonValue(_app_id)},
			{"device_id",      UPayloadBuilder::FStringToJsonValue(_device_id)},
			{"public_address", UPayloadBuilder::FStringToJsonValue(_public_address)} 
		});

	SendRequest(UAnkrUtility::GetStatUrl() + ENDPOINT_STATS_COLLECT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnkrClient - CollectStatistics - response: %s"), *content);
		}, false);
}

FString UAnkrClient::GetLastRequest()
{
	return UAnkrUtility::GetLastRequest();
}

void UAnkrClient::SetLastRequest(FString _lastRequest)
{
	UAnkrUtility::SetLastRequest(_lastRequest);
}