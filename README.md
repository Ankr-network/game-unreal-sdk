
<h3 align="center">Unreal SDK</h3>

  <p align="center">
    The Unreal Engine is aimed at Developers wanting to implement blockchain features into their game. The Unreal Engine allows the game to connect and interact with the blockchain.
    <br />
    <!-- <a href="https://github.com/github_username/repo_name"></a> -->
    <br />
    <br />
    <a href="https://github.com/Ankr-network/game-unreal-demo">View Demo</a>
    ·
    <!-- <a href="https://github.com/github_username/repo_name/issues">Report Bug</a>
    ·
    <a href="https://github.com/github_username/repo_name/issues">Request Feature</a> -->
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#use-cases">Use Cases</a></li>
        <li><a href="#prerequisites">Prerequisites</a></li>
      </ul>
    </li>
    <li><a href="#01-connect-wallet">Connect Wallet</a></li>
    <li>
      <a href="#02-performing-updates-to-nfts">Performing Updates to NFTs</a>
      <ul>
        <li><a href="#signing-transactions">Signing Transactions</a></li>
        <li><a href="#sending-transactions">Sending Transactions</a></li>
        <li><a href="#checking-status-of-ticket">Checking status of Ticket</a></li>
        <li><a href="#getdata-function">GetData Function</a></li>
        <li><a href="#sendabi-function">SendABI Function</a></li>
        <li><a href="#signmessage-function">SignMessage Function</a></li>
        <li><a href="#getsignature-function">GetSignature Function</a></li>
        <li><a href="#verifysignature-function">VerifySignature Function</a></li>
    <li><a href="#current-erc-proposals">Current ERC Proposals</a></li>
  </ol>
</details>

<!-- GETTING STARTED -->

##  🏗 Install SDK

1. Download 'AnkrSDK.zip' package from latest [release](https://github.com/Ankr-network/game-unreal-sdk/releases).
2. Unzip AnkrSDK.zip package to your Unreal Project's Plugins folder.
3. Delete Binaries, Intermediate, Saved folders.
4. Generate Visual Studio (or Xcode) project by Right-Clicking .uproject and select Generate Visual Studio project (or Services->Generate Xcode project) respectively.
5. Open the generated Visual Studio (or Xcode) project and check if the plugin in included inside the Game project.
6. Locate your GameInstance.h if already created. If not Add C++ class from Content Browser in Unreal Engine, check 'Show All Classes' and select GameInstance. Name your class 'MyGameInstance'.
7. Open MyGameInstance.h and include the following code:

```js

   #include "AnkrClient.h"

   UPROPERTY()
   UAnkrClient* ankrClient;

   UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
   UAnkrClient* GetAnkrClient();
   ```

8. Open MyGameInstance.cpp and include the following code:

```js

UAnkrClient* UMyGameInstance::GetAnkrClient()
{
	if (ankrClient == nullptr)
	{
		ankrClient = NewObject<UAnkrClient>();
	}

	return ankrClient;
}

```
9. Add "AnkrSDK" to your Unreal Project/Source/Unreal Project/Build.cs as shown below:

```js
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AnkrSDK" });
```

10. Click Edit->Project Settings->Maps and Modes and select your newly created or already created GameInstance from the GameInstance Class dropdown.

11. Now you can call all the functions in blueprint by getting GetGameInstance->GetAnkrClient.

## 👉🏻 Getting Started

AnkrSDK is used to interact with the blockchain using the latest Unreal Engine 5+

- Front End - **Unreal Engine 5+** 
- Back End 
    - **Ankr** handles all communication with the blockchain
    - The **Unreal Engine** calls the Ankr API to interact with the blockchain.

Currently, the Unreal Engine serves three Use Cases.

### 💫 Use Cases

1. Connect Wallet (MetaMask) and Authenticate User
2. Update NFT by signing and sending Transactions
3. Wearables NFT (Minting)

### 🧰 Prerequisites

1. Smart Contracts must be deployed on the blockchain.  
2. Smart Contract addresses and ABI

## 👝 01 Connect Wallet

1. Upon initialization, a unique deviceId is generated.  

```js
deviceId = load->UniqueId;
```

2. ConnectWallet is used to connect to the wallet app on your mobile device, on desktop a QR Code will be generated at the time the login button is pressed.
The session is saved to a variable for later use.

```js

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

```

2. If Login is required, wallet app will be opened on your mobile device to connect to.

```js
FPlatformProcess::LaunchURL(recievedUri.GetCharArray().GetData(), NULL, NULL);

```

3. If already logged in then GetWalletInfo is used to the get the wallet information.

```js
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

```

<p align="right">(<a href="#top">back to top</a>)</p>

### Sending Transactions

SendTransaction is used to send a transaction and a ticket will be generated.
Metamask will show popup to confirm the ticket generated by the transaction.

```js

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

```
 
If you want to send transactions, use the function `SendTransaction` with parameters having, `device_id`, `contract_address`, `abi_hash`, `method` and `args`.

### Checking Status of Ticket

Check status of tickets by calling `GetTicketResult`.

For example, if you come back to the game, check the status of the ticket with GetTicketResult function with parameters `device_id`, `ticket`.

Returns the status of the result.

Successful or unsuccessful.

### Call Method

CallMethod is used to call a function from the contract and is only readable so will not change the state of the contract.

```js

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

```

### Send ABI

SendABI is used to get the hash of the abi. 

```js

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

```

### Sign Message

SignMessage is used to sign a message
Metamask will show popup to sign or cancel the signing of the message.

```js

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

```

### Get Signature

GetSignature is used to get the result whether the user has signed the message or cancelled the signing.

```js

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

```

### Verify Signature

VerifyMessage is used to verify if the connected user has signed the message.

```js

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

```

## 🚀 02 Performing Updates to NFTs 

Getting NFTs and Making updates to the NFT.

### Get NFT Info

GetNFTInfo is used to get the NFT information.

```js

void UUpdateNFTExample::GetNFTInfo(FString abi_hash, int tokenId, FAnkrCallCompleteDynamicDelegate Result)
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
		UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - GetNFTInfo - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);
			
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			Result.ExecuteIfBound(content, content, "", -1, false);
		}
	});

	FString getTokenDetailsMethodName = "getTokenDetails";
	FString body = "{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + ContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + getTokenDetailsMethodName + "\", \"args\": \"" + FString::FromInt(tokenId) + "\"}";
	
	FString url = AnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString(body);
	Request->ProcessRequest();
}

```

### Update NFT

UpdateNFT is used to update the information on the NFT.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UUpdateNFTExample::UpdateNFT(FString abi_hash, FItemInfoStructure _item, FAnkrCallCompleteDynamicDelegate Result)
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
		UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - UpdateNFT - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);
			
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ticket = JsonObject->GetStringField("ticket");
			Result.ExecuteIfBound(content, ticket, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
		}
	});

	AnkrUtility::SetLastRequest("UpdateNFT");

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, abi_hash, _item]()
	{
		FItemInfoStructure item = _item;

		FRequestBodyStruct body{};
		body.device_id		  = deviceId;
		body.contract_address = ContractAddress;
		body.abi_hash		  = abi_hash;
		body.method			  = "updateTokenWithSignedMessage";
		body.args.Add(item);

		FString url = AnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION;
		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
		Request->SetContentAsString(FRequestBodyStruct::ToJson(body));
		Request->ProcessRequest();
	});
}

```

## 🚀 03 Wearables NFT

### Mint Character

MintCharacter is used to mint a character to the wallet address.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::MintCharacter(FString abi_hash, FString to, FAnkrCallCompleteDynamicDelegate Result)
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
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - MintCharacter - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

		FString data = content;
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ticket = JsonObject->GetStringField("ticket");
			data = ticket;
		}
			
		AnkrUtility::SetLastRequest("MintCharacter");
		Result.ExecuteIfBound(content, data, "", -1, false);
	});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, abi_hash, to]()
	{
		FString safeMintMethodName = "safeMint";

		FString url = AnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION;
		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
		Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + safeMintMethodName + "\", \"args\": [\"" + to + "\"]}");
		Request->ProcessRequest();

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
	});
}

```

### Mint Items

MintItems is used to mint items in a batch.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::MintItems(FString abi_hash, FString to, FAnkrCallCompleteDynamicDelegate Result)
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
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - MintItems - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

		FString data = content;
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ticket = JsonObject->GetStringField("ticket");
			data = ticket;
		}
			
		AnkrUtility::SetLastRequest("MintItems");
		Result.ExecuteIfBound(content, data, "", -1, false);
	});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, abi_hash, to]()
	{
		FString mintBatchMethodName = "mintBatch";

		FString args = "[\"" + to + "\", [\"" + BlueHatAddress + "\", \"" + RedHatAddress + "\", \"" + BlueShoesAddress + "\", \"" + WhiteShoesAddress + "\", \"" + RedGlassesAddress + "\", \"" + WhiteGlassesAddress + "\"], [1, 2, 3, 4, 5, 6], \"0x\"]";
		args = args.Replace(TEXT(" "), TEXT(""));

		FString url = AnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION;
		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
		Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameItemContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + mintBatchMethodName + "\", \"args\": " + args + "}");
		Request->ProcessRequest();

#if PLATFORM_ANDROID || PLATFORM_IOS
		FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
	});
}

```

### Game Item Set Approval

GameItemSetApproval is used to set an approval for the call operator.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::GameItemSetApproval(FString abi_hash, FString callOperator, bool approved, FAnkrCallCompleteDynamicDelegate Result)
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
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GameItemSetApproval - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);
			
		FString data = content;
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			bool result = JsonObject->GetBoolField("result");
			if (result)
			{
				FString ticket = JsonObject->GetStringField("ticket");
				data = ticket;
			}
		}
			
		AnkrUtility::SetLastRequest("GameItemSetApproval");
		Result.ExecuteIfBound(content, data, "", -1, false);
	});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, abi_hash, callOperator, approved]()
	{
		FString setApprovalForAllMethodName = "setApprovalForAll";

		FString body = "{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameItemContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + setApprovalForAllMethodName + "\", \"args\": [\"" + GameCharacterContractAddress + "\", true ]}";
			
		FString url = AnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION;
		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
		Request->SetContentAsString(body);
		Request->ProcessRequest();

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
	});
}

```

### Get Character Balance

GetCharacterBalance is used to get the token balance that the user has in their wallet.
The 'data' shows the number of tokens that the user holds.

```js

void UWearableNFTExample::GetCharacterBalance(FString abi_hash, FString address, FAnkrCallCompleteDynamicDelegate Result)
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
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterBalance - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);
			
		FString data = content;
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			data = JsonObject->GetStringField("data");
		}
			
		Result.ExecuteIfBound(content, data, "", -1, false);
	});

	FString balanceOfMethodName = "balanceOf";

	FString url = AnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + balanceOfMethodName + "\", \"args\": [\"" + address + "\"]}");
	Request->ProcessRequest();
}

```

### Get Character Token Id

GetCharacterTokenId is used to get the token Id at the specified index.
The 'data' shows the id of the character.

```js

void UWearableNFTExample::GetCharacterTokenId(FString abi_hash, int tokenBalance, FString owner, FString index, FAnkrCallCompleteDynamicDelegate Result)
{
	if (tokenBalance <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterTokenId - You don't own any of these tokens - tokenBalance: %d"), tokenBalance);
		return;
	}

	http = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		const FString content = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterTokenId - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);
			
		FString data = content;
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			data = JsonObject->GetStringField("data");
		}

		Result.ExecuteIfBound(content, data, "", -1, false);
	});

	FString tokenOfOwnerByIndexMethodName = "tokenOfOwnerByIndex";

	FString url = AnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + tokenOfOwnerByIndexMethodName + "\", \"args\": [\"" + owner + "\", \"" + index + "\"]}");
	Request->ProcessRequest();
}

```

### Change Hat

ChangeHat is used to change the hat of the character to another available hat.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::ChangeHat(FString abi_hash, int characterId, bool hasHat, FString hatAddress, FAnkrCallCompleteDynamicDelegate Result)
{
	if (!hasHat || characterId == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - ChangeHat - CharacterID or HatID is null"));
		return;
	}

	http = &FHttpModule::Get();
	
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, this, hatAddress](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		const FString content = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - ChangeHat - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);
			
		FString ticket = content;
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			ticket = JsonObject->GetStringField("ticket");
		}
			
		if		(hatAddress.Equals(BlueHatAddress)) AnkrUtility::SetLastRequest("ChangeHatBlue");
		else if (hatAddress.Equals(RedHatAddress))  AnkrUtility::SetLastRequest("ChangeHatRed");
			
		Result.ExecuteIfBound(content, ticket, "", -1, false);
	});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, abi_hash, characterId, hasHat, hatAddress]()
	{
		FString changeHatMethodName = "changeHat";

		FString body = "{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + changeHatMethodName + "\", \"args\": [\"" + FString::FromInt(characterId) + "\", \"" + hatAddress + "\"]}";

		FString url = AnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION;
		Request->SetURL(url);
		Request->SetVerb("POST");
		Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
		Request->SetContentAsString(body);
		Request->ProcessRequest();

#if PLATFORM_ANDROID || PLATFORM_IOS
		FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
	});
}

```

### Get Hat

GetHat is used to get the current hat of the character.
The 'data' shows the token address that the player has.

```js

void UWearableNFTExample::GetHat(FString abi_hash, int characterId, FAnkrCallCompleteDynamicDelegate Result)
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
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetHat - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

		FString data = content;
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			data = JsonObject->GetStringField("data");
		}
			
		Result.ExecuteIfBound(content, data, "", -1, false);
	});

	FString getHatMethodName = "getHat";

	FString url = AnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + getHatMethodName + "\", \"args\": [\"" + FString::FromInt(characterId) + "\"]}");
	Request->ProcessRequest();
}

```

### Get Ticket Result

GetTicketResult is used to get the result of the ticket.
The 'status' shows whether the result for the ticket signed has a success with a transaction hash.
The 'code' shows a code number related to a specific failure or success.

```js

void UWearableNFTExample::GetTicketResult(FString ticketId, FAnkrCallCompleteDynamicDelegate Result)
{
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = http->CreateRequest();
#endif
	Request->OnProcessRequestComplete().BindLambda([Result, ticketId, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		const FString content = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetTicketResult - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);
			
		FString data = content;
		int code = 0;
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			code = 1;

			if (AnkrUtility::GetLastRequest().Equals("ChangeHatBlue") || AnkrUtility::GetLastRequest().Equals("ChangeHatRed"))
			{
				bool result					   = JsonObject->GetBoolField("result");
				TSharedPtr<FJsonObject> object = JsonObject->GetObjectField("data");
				FString transactionHash		   = object->GetStringField("tx_hash");
				FString status				   = object->GetStringField("status");
				UE_LOG(LogTemp, Warning, TEXT("tx_hash: %s | status: %s"), *transactionHash, *status);

				if (result && status == "success")
				{
					code = 123;
				}
			}
		}

		Result.ExecuteIfBound(content, data, "", code, false);
	});

	FString url = AnkrUtility::GetUrl() + ENDPOINT_RESULT;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"ticket\": \"" + ticketId + "\" }");
	Request->ProcessRequest();
}

```

### Get Items Balance

GetItemsBalance is used to get the balance of items in a batch.
The 'data' shows a response of an array of balances for each token, in the sequence that were sent as a request.

```js

void UWearableNFTExample::GetItemsBalance(FString abi_hash, FString address, FAnkrCallCompleteDynamicDelegate Result)
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
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetItemsBalance - GetContentAsString: %s"), *content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

		FString data = content;
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			data = JsonObject->GetStringField("data");
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetItemsBalance - Balance: %s"), *data);
		}
			
		Result.ExecuteIfBound(content, data, "", -1, false);
	});

	FString balanceOfBatchMethodName = "balanceOfBatch";

	FString args = "[ [\"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\"], [\"" + BlueHatAddress + "\", \"" + RedHatAddress + "\", \"" + WhiteHatAddress + "\", \"" + BlueShoesAddress + "\", \"" + RedShoesAddress + "\", \"" + WhiteShoesAddress + "\", \"" + BlueGlassesAddress + "\", \"" + RedGlassesAddress + "\", \"" + WhiteGlassesAddress + "\"]]";
	
	FString url = AnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD;
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(CONTENT_TYPE_KEY, CONTENT_TYPE_VALUE);
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameItemContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + balanceOfBatchMethodName + "\", \"args\": " + args + "}");
	Request->ProcessRequest();
}

```

<p align="right">(<a href="#top">back to top</a>)</p>

## 🗒 Current ERC Proposals

We have two ERC proposals.  
[ERC-4884  Rentable NFT Standard](https://github.com/Ankr-network/game-smart-contract-example/blob/master/ERC/rentable-nft.md)
[ERC-4911  Composability Extension For ERC-721 Standard](https://github.com/Ankr-network/game-smart-contract-example/blob/master/ERC/composable-nft.md)


<p align="right">(<a href="#top">back to top</a>)</p>




