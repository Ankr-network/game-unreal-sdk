
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
2. Unzip AnkrSDK.zip package to your Unreal Project/Plugins folder.
3. Delete Binaries, Intermediate, Saved folders and Visual Studio Project file (.sln) from Unreal Project (If any).
4. Generate Visual Studio Project by Right-Clicking .uproject and select Generate Visual Studio project files.
5. Open the generated Visual Studio solution file (.sln) and check if the plugin in included in the solution explorer.
6. Locate your GameInstance.h if already created. If not Add C++ class from Content Browser, check 'Show All Classes' and select GameInstance. Name your class 'MyGameInstance'.
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

The Unreal Engine gets the backend client to connect and interact with the blockchain to send and retrieve data.

The baseURL is: http://45.77.189.28:5000/

- Front End - **Unreal Engine 5.0** 
- Back End 
    - **Ankr** handles all communication with the backend and blockchain via a baseURL
    - The **Unreal Engine** sends call functions and parameters and receives JSON responses as to whether the call is successful or not.

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
baseUrl = "http://45.77.189.28:5000/";
```

2. GetClient is used to send a request with 'device_id' as a raw body parameter at http://45.77.189.28:5000/connect to get a response having a 'uri' deeplink to openmetamask, a 'session' and a 'login' information if required.
The 'uri' deeplink will only work on mobile devices as on desktop a QR Code will be generated at the time the login button is pressed.
The session is saved to a variable for later use.

```js

bool UAnkrClient::GetClient(FAnkrConnectionStatus Status)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Status, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString recievedUri = JsonObject->GetStringField("uri");
				FString sessionId	= JsonObject->GetStringField("session");
				bool needLogin		= JsonObject->GetBoolField("login");
				session				= sessionId;

				updateNFTExample->Init(deviceId, baseUrl, session);
				wearableNFTExample->Init(deviceId, baseUrl, session);

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

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\"}");
	Request->ProcessRequest();
	return true;
}

```

2. If Login via MetaMask is required to authenticate the session. A connection request is sent to metamask with `uri` as a deeplink.

```js
FPlatformProcess::LaunchURL(recievedUri.GetCharArray().GetData(), NULL, NULL);

```

3. If already logged in then a JSON object is returned with the `session_id` and `URI`. This is now a valid session and any transaction to the blockchain can be performed.  

<p align="right">(<a href="#top">back to top</a>)</p>

### Sending Transactions

SendTransaction is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args' } as a raw body parameter at http://45.77.189.28:5000/send/transaction to get a response having a 'ticket'.
The session saved during GetClient will be used to open metamask.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UAnkrClient::SendTransaction(FString contract, FString abi_hash, FString method, FString args, FAnkrTicket Ticket)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Ticket, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString ticketId = JsonObject->GetStringField("ticket");
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
			Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");
			Request->ProcessRequest();
		});

#if PLATFORM_ANDROID
	FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
}

```
 
If you want to send transactions, use the function `SendTransaction` with body parameters having, `device_id`, `contract_address`, `abi_hash`, `method` and `args`.

When a call is made, the back end client sends a `Ticket` via MetaMask if the call was successful.

### Checking Status of Ticket

Check status of tickets by calling `GetTicketResult`.

For example, if you come back to the game, check the status of the ticket with GetTicketResult having body parameters as `device_id`, `ticket`.

Returns the status of the result.

Successful or unsuccessful.

### Get Data

GetData is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args' } as a raw body parameter at http://45.77.189.28:5000/wallet/call/method to get a response data from readable functions from the contract.

```js

void UAnkrClient::GetData(FString contract, FString abi_hash, FString method, FString args, FAnkrDelegate Result)
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
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");
	Request->ProcessRequest();
}

```

### Send ABI

SendABI is used to send a request with 'abi' as a raw body parameter at http://45.77.189.28:5000/abi to get a response 'abi' which is the abi hash generated by the backend. 

```js

void UAnkrClient::SendABI(FString abi, FAnkrDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();
			
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
	
	Request->SetContentAsString(*body);
	Request->ProcessRequest();
}

```

### Sign Message

SignMessage is used to send a request with { 'device_id', 'message' } as a raw body parameter at http://45.77.189.28:5000/sign/message to get a response having a 'ticket'.
The session saved during GetClient will be used to open metamask.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UAnkrClient::SignMessage(FString message, FAnkrDelegate Result)
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

```

A `ticketid` is returned. 

The Call is as follows:

```js
FString url = baseUrl + "sign/message";
```

### Get Signature

GetSignature is used to send a request with { 'device_id', 'ticket' } as a raw body parameter at http://45.77.189.28:5000/result to get a response having a 'data' object with 'signature' string field.
The signature will used in addition to the message for verification.

```js

void UAnkrClient::GetSignature(FString ticket, FAnkrDelegate Result)
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
				Result.ExecuteIfBound(data->GetStringField("signature"));
			}
		});
	
	FString url = baseUrl + "result";

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"ticket\":\"" + ticket + "\"}");
	Request->ProcessRequest();
}

```

### Verify Signature

VerifyMessage is used to send a request with { 'device_id', 'message', 'signature' } as a raw body parameter at http://45.77.189.28:5000/verify/message to get a response having an account 'address'.
The account address will the connected wallet's public address.

```js

void UAnkrClient::VerifyMessage(FString message, FString signature, FAnkrDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				Result.ExecuteIfBound(JsonObject->GetStringField("address"));
			}
		});

	FString url = baseUrl + "verify/message";
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"message\":\"" + message + "\", \"signature\":\"" + signature + "\"}");
	Request->ProcessRequest();
}

```

## 🚀 02 Performing Updates to NFTs 

Getting NFTs and Making updates to the NFT.

### Get NFT Info

GetNFTInfo is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args': "tokenId" } as a raw body parameter at http://45.77.189.28:5000/call/method to get a response having a data { 'tokenId', 'itemType', 'strength', 'level', 'expireTime', 'signature' }.

```js

void UUpdateNFTExample::GetNFTInfo(FString abi_hash, int tokenId, FAnkrDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - GetNFTInformation - GetContentAsString: %s"), *Response->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				Result.ExecuteIfBound(Response->GetContentAsString());
			}
		});

	FString getTokenDetailsMethodName = "getTokenDetails";
	FString content = "{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + ContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + getTokenDetailsMethodName + "\", \"args\": \"" + FString::FromInt(tokenId) + "\"}";
	UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - GetNFTInformation - content: %s"), *content);

	FString url = baseUrl + "call/method";

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString(content);
	Request->ProcessRequest();
}

```

### Update NFT

UpdateNFT is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args': { 'tokenId':int, 'itemType':int, 'strength':int, 'level':int, 'expireTime':int, 'signature':"0x" } } as a raw body parameter at http://45.77.189.28:5000/send/transaction to get a response having a 'ticket'.
The session saved during Init will be used to open metamask.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UUpdateNFTExample::UpdateNFT(FString abi_hash, FItemInfoStructure _item, FAnkrDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString ticket = JsonObject->GetStringField("ticket");
				
				Result.ExecuteIfBound(ticket);

#if PLATFORM_ANDROID
				FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
			}
		});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, abi_hash, _item]()
		{
			FString url = baseUrl + "send/transaction";
			Request->SetURL(url);
			Request->SetVerb("POST");
			Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
			Request->SetHeader("Content-Type", TEXT("application/json"));

			FRequestBodyStruct requestBody{};
			requestBody.device_id = deviceId;
			requestBody.contract_address = ContractAddress;
			requestBody.abi_hash = abi_hash;
			requestBody.method = "updateTokenWithSignedMessage";

			FItemInfoStructure item = _item;
			requestBody.args.Add(item);

			Request->SetContentAsString(FRequestBodyStruct::ToJson(requestBody));
			Request->ProcessRequest();
		});
}

```

## 🚀 03 Wearables NFT

### Mint Character

MintCharacter is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args': ["0xto"] } as a raw body parameter at http://45.77.189.28:5000/send/transaction to get a response having a 'ticket'.
The session saved during Init will be used to open metamask.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::MintCharacter(FString abi_hash, FString to, FAnkrDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();
			
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString ticket = JsonObject->GetStringField("ticket");
				data = ticket;
				
			}
			lastMethod = "MintCharacter";
			Result.ExecuteIfBound(data);
		});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, abi_hash, to]()
		{
			FString safeMintMethodName = "safeMint";

			FString url = baseUrl + "send/transaction";
			Request->SetURL(url);
			Request->SetVerb("POST");
			Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
			Request->SetHeader("Content-Type", TEXT("application/json"));
			Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + safeMintMethodName + "\", \"args\": [\"" + to + "\"]}");
			Request->ProcessRequest();

#if PLATFORM_ANDROID
			FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
		});
}

```

### Mint Items

MintItems is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args': ["0xpubAddres\", ["0xtokAddres", "0xtokAddres", "0xtokAddres", "0xtokAddres", "0xtokAddres", "0xtokAddres"],[1, 2, 3, 4, 5, 6], []] } as a raw body parameter at http://45.77.189.28:5000/send/transaction to get a response having a 'ticket'.
The session saved during Init will be used to open metamask.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::MintItems(FString abi_hash, FString to, FAnkrDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString ticket = JsonObject->GetStringField("ticket");
				data = ticket;

			}
			lastMethod = "MintItems";
			Result.ExecuteIfBound(data);
		});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, abi_hash, to]()
		{
			FString mintBatchMethodName = "mintBatch";

			FString json = "[\"" + to + "\", [\"" + BlueHatAddress + "\", \"" + RedHatAddress + "\", \"" + BlueShoesAddress + "\", \"" + WhiteShoesAddress + "\", \"" + RedGlassesAddress + "\", \"" + WhiteGlassesAddress + "\"], [1, 2, 3, 4, 5, 6], \"0x\"]";
			json = json.Replace(TEXT(" "), TEXT(""));

			FString url = baseUrl + "send/transaction";
			Request->SetURL(url);
			Request->SetVerb("POST");
			Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
			Request->SetHeader("Content-Type", TEXT("application/json"));
			Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameItemContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + mintBatchMethodName + "\", \"args\": " + json + "}");
			Request->ProcessRequest();

#if PLATFORM_ANDROID
			FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
		});
}

```

### Game Item Set Approval

GameItemSetApproval is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args': ["0xoperatorContractAddress", true] } as a raw body parameter at http://45.77.189.28:5000/send/transaction to get a response having a 'ticket'.
The session saved during Init will be used to open metamask.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::GameItemSetApproval(FString abi_hash, FString callOperator, bool approved, FAnkrDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				bool result = JsonObject->GetBoolField("result");
				if (result)
				{
					FString ticket = JsonObject->GetStringField("ticket");
					data = ticket;
				}
			}
			lastMethod = "GameItemSetApproval";
			Result.ExecuteIfBound(data);
		});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, abi_hash, callOperator, approved]()
		{
			FString setApprovalForAllMethodName = "setApprovalForAll";

			FString url = baseUrl + "send/transaction";
			Request->SetURL(url);
			Request->SetVerb("POST");
			Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
			Request->SetHeader("Content-Type", TEXT("application/json"));
			FString body = "{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameItemContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + setApprovalForAllMethodName + "\", \"args\": [\"" + GameCharacterContractAddress + "\", true ]}";
			
			Request->SetContentAsString(body);
			Request->ProcessRequest();

#if PLATFORM_ANDROID
			FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
		});
}

```

### Get Character Balance

GetCharacterBalance is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args': ["0xpubAddres"] } as a raw body parameter at http://45.77.189.28:5000/call/method to get a response having a 'data'.
The 'data' shows the number of tokens that the user holds.

```js

void UWearableNFTExample::GetCharacterBalance(FString abi_hash, FString address, FAnkrDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				data = JsonObject->GetStringField("data");
			}
			lastMethod = "GetCharacterBalance";
			Result.ExecuteIfBound(data);
		});

	FString balanceOfMethodName = "balanceOf";

	FString url = baseUrl + "call/method";
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + balanceOfMethodName + "\", \"args\": [\"" + address + "\"]}");
	Request->ProcessRequest();
}

```

### Get Character Token Id

GetCharacterTokenId is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args': ["0xpubAddres", "index"] } as a raw body parameter at http://45.77.189.28:5000/result to get a response having a 'data' object field.
The 'data' shows the id of the character.

```js

void UWearableNFTExample::GetCharacterTokenId(FString abi_hash, int tokenBalance, FString owner, FString index, FAnkrDelegate Result)
{
	if (tokenBalance <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterTokenId - You don't own any of these tokens - tokenBalance: %d"), tokenBalance);
		return;
	}

	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				data = JsonObject->GetStringField("data");
			}
			lastMethod = "GetCharacterTokenId";
			Result.ExecuteIfBound(data);
		});

	FString tokenOfOwnerByIndexMethodName = "tokenOfOwnerByIndex";

	FString url = baseUrl + "call/method";
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + tokenOfOwnerByIndexMethodName + "\", \"args\": [\"" + owner + "\", \"" + index + "\"]}");
	Request->ProcessRequest();
}

```

### Change Hat

ChangeHat is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args': ["characterId", "tokenAddress"] } as a raw body parameter at http://45.77.189.28:5000/send/transaction to get a response having a 'ticket'.
The session saved during Init will be used to open metamask.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::ChangeHat(FString abi_hash, int characterId, bool hasHat, FString hatAddress, FAnkrDelegate Result)
{
	if (!hasHat || characterId == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - ChangeHat - CharacterID or HatID is null"));
		return;
	}

	http = &FHttpModule::Get();
	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString ticket = Response->GetContentAsString();

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				ticket = JsonObject->GetStringField("ticket");
			}
			lastMethod = "ChangeHat";
			if (hat.Equals(BlueHatAddress))
			{
				lastMethod = "ChangeHatBlue";
			}
			else if (hat.Equals(RedHatAddress))
			{
				lastMethod = "ChangeHatRed";
			}
			Result.ExecuteIfBound(ticket);
		});

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request, abi_hash, characterId, hasHat, hatAddress]()
		{
			hat = hatAddress;
			FString changeHatMethodName = "changeHat";

			FString url = baseUrl + "send/transaction";
			Request->SetURL(url);
			Request->SetVerb("POST");
			Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
			Request->SetHeader("Content-Type", TEXT("application/json"));
			FString body = "{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + changeHatMethodName + "\", \"args\": [\"" + FString::FromInt(characterId) + "\", \"" + BlueHatAddress + "\"]}";
			
			Request->SetContentAsString(body);
			Request->ProcessRequest();

#if PLATFORM_ANDROID
			FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
		});
}

```

### Get Hat

GetHat is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args': ["characterId"] } as a raw body parameter at http://45.77.189.28:5000/call/method to get a response having a 'data' string field.
The 'data' shows the token address that the player has.

```js

void UWearableNFTExample::GetHat(FString abi_hash, int characterId, FAnkrDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				data = JsonObject->GetStringField("data");
			}
			lastMethod = "GetHat";
			Result.ExecuteIfBound(data);
		});

	FString getHatMethodName = "getHat";

	FString url = baseUrl + "call/method";

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + getHatMethodName + "\", \"args\": [\"" + FString::FromInt(characterId) + "\"]}");
	Request->ProcessRequest();
}

```

### Get Ticket Result

GetTicketResult is used to send a request with { 'ticket' } as a raw body parameter at http://45.77.189.28:5000/result to get a response having a 'data' string field.
The 'status' shows whether the result for the ticket signed has a success with a transaction hash.
The 'code' shows a code number related to a specific failure or success.

```js

void UWearableNFTExample::GetTicketResult(FString ticketId, FAnkrTicketResult Result)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, ticketId, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString data = JsonObject->GetStringField("data");
				int code = 1;
				
				if (lastMethod.Equals("ChangeHatBlue") || lastMethod.Equals("ChangeHatRed"))
				{
					bool result = JsonObject->GetBoolField("result");
					TSharedPtr<FJsonObject> object = JsonObject->GetObjectField("data");
					FString transactionHash = object->GetStringField("tx_hash");
					FString status = object->GetStringField("status");
					
					if (result && status == "success")
					{
						code = 123;
					}
				}
				Result.ExecuteIfBound(Response->GetContentAsString(), code);
			}
		});

	FString url = baseUrl + "result";
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"ticket\": \"" + ticketId + "\" }");
	Request->ProcessRequest();
}

```

### Get Items Balance

GetItemsBalance is used to send a request with { 'device_id', 'contract_address', 'abi_hash', 'method', 'args': [["9 wallet address elements"], ["9 token address elements"]] } as a raw body parameter at http://45.77.189.28:5000/call/method to get a response having a 'data' string field.
The 'data' shows a response of an array of balances for each token, in the sequence that were sent as a request.

```js

void UWearableNFTExample::GetItemsBalance(FString abi_hash, FString address, FAnkrDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FString data = Response->GetContentAsString();

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				data = JsonObject->GetStringField("data");
			}
			lastMethod = "GetItemsBalance";
			Result.ExecuteIfBound(data);
		});

	FString balanceOfBatchMethodName = "balanceOfBatch";

	FString body = "[ [\"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\", \"" + activeAccount + "\"], [\"" + BlueHatAddress + "\", \"" + RedHatAddress + "\", \"" + WhiteHatAddress + "\", \"" + BlueShoesAddress + "\", \"" + RedShoesAddress + "\", \"" + WhiteShoesAddress + "\", \"" + BlueGlassesAddress + "\", \"" + RedGlassesAddress + "\", \"" + WhiteGlassesAddress + "\"]]";
	
	FString url = baseUrl + "call/method";

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\", \"contract_address\": \"" + GameItemContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + balanceOfBatchMethodName + "\", \"args\": " + body + "}");
	Request->ProcessRequest();
}

```

<p align="right">(<a href="#top">back to top</a>)</p>

## 🗒 Current ERC Proposals

We have two ERC proposals.  
[ERC-4884  Rentable NFT Standard](https://github.com/Ankr-network/game-smart-contract-example/blob/master/ERC/rentable-nft.md)
[ERC-4911  Composability Extension For ERC-721 Standard](https://github.com/Ankr-network/game-smart-contract-example/blob/master/ERC/composable-nft.md)


<p align="right">(<a href="#top">back to top</a>)</p>




