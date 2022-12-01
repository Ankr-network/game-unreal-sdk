
<h3 lign="center">Unreal SDK</h3>

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

1. Download 'MirageSDK.zip' package from latest [release](https://github.com/Ankr-network/game-unreal-sdk/releases).
2. Unzip MirageSDK.zip package to your Unreal Project's Plugins folder.
3. Delete Binaries, Intermediate, Saved folders.
4. Generate Visual Studio (or Xcode) project by Right-Clicking .uproject and select Generate Visual Studio project (or Services->Generate Xcode project) respectively.
5. Open the generated Visual Studio (or Xcode) project and check if the plugin in included inside the Game project.
6. There are two ways you can create an instance of MirageClient object which are shown below:

(6a) Open any of your blueprint, call "Construct Object From Class", select MirageClient and store the reference in a variable.

<img width="517" alt="InstanceCreation" src="https://user-images.githubusercontent.com/99165088/189541606-8049b5c2-bfba-42df-989d-7bb424d9389c.png">

(6b) Locate your GameInstance.h if already created. If not Add C++ class from Content Browser in Unreal Engine, check 'Show All Classes' and select GameInstance. Name your class 'MyGameInstance'.

7. Open MyGameInstance.h and include the following code:

```js

   #include "MirageClient.h"

   UPROPERTY()
   UMirageClient* mirageClient;

   UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
   UMirageClient* GetMirageClient();
   ```

8. Open MyGameInstance.cpp and include the following code:

```js

UMirageClient* UMyGameInstance::GetMirageClient()
{
	if (mirageClient == nullptr)
	{
		mirageClient = NewObject<UMirageClient>();
	}

	return mirageClient;
}

```
9. Add "MirageSDK" to your Unreal Project/Source/Unreal Project/Build.cs as shown below:

```js
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "MirageSDK" });
```

10. Click Edit->Project Settings->Maps and Modes and select your newly created or already created GameInstance from the GameInstance Class dropdown.

11. Now you can call all the functions in blueprint by getting GetGameInstance->GetMirageClient.

## 👉🏻 Getting Started

MirageSDK is used to interact with the blockchain using the latest Unreal Engine 5+

- Front End - **Unreal Engine 5+** 
- Back End 
    - **Mirage** handles all communication with the blockchain
    - The **Unreal Engine** calls the Mirage API to interact with the blockchain.

Currently, the Unreal Engine serves three Use Cases.

### 💫 Use Cases

1. Connect Wallet (MetaMask) and Authenticate User
2. Update NFT by signing and sending Transactions
3. Wearables NFT (Minting)

### 🧰 Prerequisites

1. Smart Contracts must be deployed on the blockchain.  
2. Smart Contract addresses and ABI

## 👝 01 Connect Wallet

ConnectWallet is used to connect to the wallet app on your mobile device, on desktop a QR Code will be generated at the time the login button is pressed.
The session is saved to a variable for later use.

To Connect wallet call the C++ ConnectWallet in Blueprint shown below:

```js

void UMirageClient::ConnectWallet(const FMirageCallCompleteDynamicDelegate& Result);

```
<img width="494" alt="ConnectWalletpng" src="https://user-images.githubusercontent.com/99165088/187159950-fc5d030a-012d-42cb-bdfa-3383867f48b3.png">

2. If Login is required, wallet app will be opened on your mobile device to connect to.

3. If already logged in then GetWalletInfo is used to the get the wallet information.

To Get wallet information call the C++ GetWalletInfo in Blueprint shown below:

```js

void UMirageClient::GetWalletInfo(const FMirageCallCompleteDynamicDelegate& Result);

```
<img width="529" alt="GetWalletpng" src="https://user-images.githubusercontent.com/99165088/187161391-da60ae1c-38a5-47e9-9553-94a5a6349a2a.png">

<p align="right">(<a href="#top">back to top</a>)</p>

### Sending Transactions

SendTransaction is used to send a transaction and a ticket will be generated.
Metamask will show popup to confirm the ticket generated by the transaction.

```js

void UMirageClient::SendTransaction(FString contract, FString abi_hash, FString method, FString args, const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id",	     UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID())},
			{"contract_address", UPayloadBuilder::FStringToJsonValue(contract)},
			{"abi_hash",	     UPayloadBuilder::FStringToJsonValue(abi_hash)},
			{"method",	     UPayloadBuilder::FStringToJsonValue(method)},
			{"args",	     UPayloadBuilder::FStringToJsonValue(args)} 
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

void UMirageClient::CallMethod(FString contract, FString abi_hash, FString method, FString args, const FMirageCallCompleteDynamicDelegate& Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{"device_id",	     UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID())},
			{"contract_address", UPayloadBuilder::FStringToJsonValue(contract)},
			{"abi_hash",	     UPayloadBuilder::FStringToJsonValue(abi_hash)},
			{"method",	     UPayloadBuilder::FStringToJsonValue(method)},
			{"args",	     UPayloadBuilder::FStringToJsonValue(args)} 
		});
	
	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("MirageClient - CallMethod - response: %s"), *content);

			callback.ExecuteIfBound(content, content, "", -1, false);

		}, Result, false);
}

```

### Send ABI

SendABI is used to get the hash of the abi. 

```js

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

```

### Sign Message

SignMessage is used to sign a message
Metamask will show popup to sign or cancel the signing of the message.

```js

void UMirageClient::SignMessage(FString message, const FMirageCallCompleteDynamicDelegate & Result)
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

```

### Get Signature

GetSignature is used to get the result whether the user has signed the message or cancelled the signing.

```js

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

```

### Verify Signature

VerifyMessage is used to verify if the connected user has signed the message.

```js

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

```

## 🚀 02 Performing Updates to NFTs 

Getting NFTs and Making updates to the NFT.

### Get NFT Info

GetNFTInfo is used to get the NFT information.

```js

void UUpdateNFTExample::GetNFTInfo(FString abi_hash, int tokenId, FMirageCallCompleteDynamicDelegate Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{ 
			{ "device_id",	     UPayloadBuilder::FStringToJsonValue(UMirageUtility::GetDeviceID())},
			{"contract_address", UPayloadBuilder::FStringToJsonValue(ContractAddress)},
			{"abi_hash",	     UPayloadBuilder::FStringToJsonValue(abi_hash)},
			{"method",	     UPayloadBuilder::FStringToJsonValue("getTokenDetails")},
			{"args",	     UPayloadBuilder::FStringToJsonValue(FString::FromInt(tokenId))} 
		});

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - GetNFTInfo - response: %s"), *content);

			callback.ExecuteIfBound(content, content, "", -1, false);

		}, Result, false);
}

```

### Update NFT

UpdateNFT is used to update the information on the NFT.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UUpdateNFTExample::UpdateNFT(FString abi_hash, FItemInfoStructure _item, FMirageCallCompleteDynamicDelegate Result)
{
	TSharedPtr<FJsonObject> item = UPayloadBuilder::GetBuilder();
	item->SetNumberField("tokenId",    _item.tokenId);
	item->SetNumberField("itemType",   _item.itemType);
	item->SetNumberField("strength",   _item.strength);
	item->SetNumberField("level",      _item.level);
	item->SetNumberField("expireTime", _item.expireTime);
	item->SetStringField("signature",  _item.signature);

	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddNestedObject(args, item);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",        UMirageUtility::GetDeviceID());
	builder->SetStringField("contract_address", ContractAddress);
	builder->SetStringField("abi_hash",	    abi_hash);
	builder->SetStringField("method",	    "updateTokenWithSignedMessage");
	builder->SetArrayField("args",		    args);
	const FString payload = UPayloadBuilder::Build(builder);
	
	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - UpdateNFT - response: %s"), *content);

			FString ticket = jsonObject->GetStringField("ticket");

			callback.ExecuteIfBound(content, ticket, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
			UMirageUtility::SetLastRequest("UpdateNFT");
			FPlatformProcess::LaunchURL(UMirageUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

		}, Result, true);
}

```

## 🚀 03 Wearables NFT

### Mint Character

MintCharacter is used to mint a character to the wallet address.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::MintCharacter(FString abi_hash, FString to, FMirageCallCompleteDynamicDelegate Result)
{
	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, to);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",	    UMirageUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameCharacterContractAddress);
	builder->SetStringField("abi_hash",	    abi_hash);
	builder->SetStringField("method",	    "safeMint");
	builder->SetArrayField ("args",	            args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - MintCharacter - response: %s"), *content);

			FString ticket = jsonObject->GetStringField("ticket");

			UMirageUtility::SetLastRequest("MintCharacter");
			callback.ExecuteIfBound(content, ticket, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(UMirageUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

		}, Result, true);
}

```

### Mint Items

MintItems is used to mint items in a batch.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::MintItems(FString abi_hash, FString to, FMirageCallCompleteDynamicDelegate Result)
{
	const TArray<FString> items = { BlueHatAddress, RedHatAddress, BlueShoesAddress, WhiteShoesAddress, RedGlassesAddress, WhiteGlassesAddress };
	
	TArray<TSharedPtr<FJsonValue>> itemsArray;
	for (int32 i = 0; i < items.Num(); i++)
	{
		UPayloadBuilder::AddArrayItem(itemsArray, items[i]);
	}

	const TArray<int> indices = { 1, 2, 3, 4, 5, 6 };

	TArray<TSharedPtr<FJsonValue>> indicesArray;
	for (int32 i = 0; i < indices.Num(); i++)
	{
		UPayloadBuilder::AddArrayItem(indicesArray, indices[i]);
	}

	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem  (args, to);
	UPayloadBuilder::AddNestedArray(args, itemsArray);
	UPayloadBuilder::AddNestedArray(args, indicesArray);
	UPayloadBuilder::AddArrayItem  (args, FString("0x"));

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",        UMirageUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameItemContractAddress);
	builder->SetStringField("abi_hash",	    abi_hash);
	builder->SetStringField("method",	    "mintBatch");
	builder->SetArrayField ("args",		    args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - MintItems - response: %s"), *content);

			FString ticket = jsonObject->GetStringField("ticket");

			UMirageUtility::SetLastRequest("MintItems");
			callback.ExecuteIfBound(content, ticket, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(UMirageUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

		}, Result, true);
}

```

### Game Item Set Approval

GameItemSetApproval is used to set an approval for the call operator.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

void UWearableNFTExample::GameItemSetApproval(FString abi_hash, FString callOperator, bool approved, FMirageCallCompleteDynamicDelegate Result)
{
	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, GameCharacterContractAddress);
	UPayloadBuilder::AddArrayItem(args, true);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",	    UMirageUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameItemContractAddress);
	builder->SetStringField("abi_hash",	    abi_hash);
	builder->SetStringField("method",	    "setApprovalForAll");
	builder->SetArrayField("args",		    args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GameItemSetApproval - response: %s"), *content);

			FString data = content;
			bool result = jsonObject->GetBoolField("result");
			if (result)
			{
				FString ticket = jsonObject->GetStringField("ticket");
				data = ticket;
			}

			UMirageUtility::SetLastRequest("GameItemSetApproval");
			callback.ExecuteIfBound(content, data, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(UMirageUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

		}, Result, true);
}

```

### Get Character Balance

GetCharacterBalance is used to get the token balance that the user has in their wallet.
The 'data' shows the number of tokens that the user holds.

```js

void UWearableNFTExample::GetCharacterBalance(FString abi_hash, FString address, FMirageCallCompleteDynamicDelegate Result)
{
	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, address);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",	    UMirageUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameCharacterContractAddress);
	builder->SetStringField("abi_hash",	    abi_hash);
	builder->SetStringField("method",	    "balanceOf");
	builder->SetArrayField("args",		    args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterBalance - response: %s"), *content);

			FString data = jsonObject->GetStringField("data");

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}

```

### Get Character Token Id

GetCharacterTokenId is used to get the token Id at the specified index.
The 'data' shows the id of the character.

```js

void UWearableNFTExample::GetCharacterTokenId(FString abi_hash, int tokenBalance, FString owner, FString index, FMirageCallCompleteDynamicDelegate Result)
{
	if (tokenBalance <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterTokenId - You don't own any of these tokens - tokenBalance: %d"), tokenBalance);
		return;
	}

	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, owner);
	UPayloadBuilder::AddArrayItem(args, index);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",	    UMirageUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameCharacterContractAddress);
	builder->SetStringField("abi_hash",	    abi_hash);
	builder->SetStringField("method",	    "tokenOfOwnerByIndex");
	builder->SetArrayField("args",		    args);
	const FString payload = UPayloadBuilder::Build(builder);

	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterTokenId - payload: %s"), *payload);

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterTokenId - response: %s"), *content);

			FString data = jsonObject->GetStringField("data");

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}

```

### Change Hat

ChangeHat is used to change the hat of the character to another available hat.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

<<<<<<< Updated upstream
void UWearableNFTExample::ChangeHat(FString abi_hash, int characterId, FString hatAddress, FAnkrCallCompleteDynamicDelegate Result)
=======
void UWearableNFTExample::ChangeHat(FString abi_hash, int characterId, bool hasHat, FString hatAddress, FMirageCallCompleteDynamicDelegate Result)
>>>>>>> Stashed changes
{
	if (characterId < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - ChangeHat - CharacterID or HatID is null"));
		return;
	}

<<<<<<< Updated upstream
	if (hatAddress.Equals(BlueHatAddress)) UAnkrUtility::SetLastRequest("ChangeHatBlue");
	else if (hatAddress.Equals(RedHatAddress)) UAnkrUtility::SetLastRequest("ChangeHatRed");
	else if (hatAddress.Equals(RedHatAddress)) UAnkrUtility::SetLastRequest("ChangeHatWhite");

	TSharedPtr<FJsonObject> gas = UPayloadBuilder::GetBuilder();
	gas->SetNumberField("gasLimit", 200000);
=======
	if (hatAddress.Equals(BlueHatAddress)) UMirageUtility::SetLastRequest("ChangeHatBlue");
	else if (hatAddress.Equals(RedHatAddress)) UMirageUtility::SetLastRequest("ChangeHatRed");
>>>>>>> Stashed changes

	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, FString::FromInt(characterId));
	UPayloadBuilder::AddArrayItem(args, hatAddress);
	//UPayloadBuilder::AddNestedObject(args, gas);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
<<<<<<< Updated upstream
	builder->SetStringField("device_id",		UAnkrUtility::GetDeviceID());
=======
	builder->SetStringField("device_id",	    UMirageUtility::GetDeviceID());
>>>>>>> Stashed changes
	builder->SetStringField("contract_address", GameCharacterContractAddress);
	builder->SetStringField("abi_hash",			abi_hash);
	builder->SetStringField("method",			"changeHat");
	builder->SetArrayField("args",				args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this, hatAddress](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - ChangeHat - response: %s"), *content);

			FString ticket = jsonObject->GetStringField("ticket");

			callback.ExecuteIfBound(content, ticket, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(UMirageUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

		}, Result, true);
}

```

### Get Hat

GetHat is used to get the current hat of the character.
The 'data' shows the token address that the player has.

```js

void UWearableNFTExample::GetHat(FString abi_hash, int characterId, FMirageCallCompleteDynamicDelegate Result)
{
	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, FString::FromInt(characterId));

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",        UMirageUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameCharacterContractAddress);
	builder->SetStringField("abi_hash",	    abi_hash);
	builder->SetStringField("method",	    "getHat");
	builder->SetArrayField("args",		    args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetHat - response: %s"), *content);

			FString data = jsonObject->GetStringField("data");

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}

```

### Get Wearable NFT Result

GetWearableNFTResult is used to get the result of the ticket.
The 'status' shows whether the result for the ticket signed has a success with a transaction hash.
The 'code' shows a code number related to a specific failure or success.

```js

void UWearableNFTExample::GetWearableNFTResult(FString ticketId, FMirageCallCompleteDynamicDelegate Result)
{
	const FString payload = UPayloadBuilder::BuildPayload(
		{
			{"ticket", UPayloadBuilder::FStringToJsonValue(ticketId)}
		});

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_RESULT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetWearableNFTResult - response: %s"), *content);

			FString status;
			FString txHash;
			bool hasTxHash = false;

<<<<<<< Updated upstream
			bool result = jsonObject->GetBoolField("result");
			if (result)
=======
			if (UMirageUtility::GetLastRequest().Equals("ChangeHatBlue") || UMirageUtility::GetLastRequest().Equals("ChangeHatRed"))
>>>>>>> Stashed changes
			{
				TSharedPtr<FJsonObject> object = jsonObject->GetObjectField("data");
				status = object->GetStringField("status");

				hasTxHash = object->TryGetStringField("tx_hash", txHash);
				if (hasTxHash)
				{
					UE_LOG(LogTemp, Warning, TEXT("tx_hash: %s"), *txHash);
				}
			}

			callback.ExecuteIfBound(content, txHash, status, -1, hasTxHash);

		}, Result, false);
}

```

### Get Items Balance

GetItemsBalance is used to get the balance of items in a batch.
The 'data' shows a response of an array of balances for each token, in the sequence that were sent as a request.

```js

void UWearableNFTExample::GetItemsBalance(FString abi_hash, FString address, FMirageCallCompleteDynamicDelegate Result)
{
	TArray<TSharedPtr<FJsonValue>> addressArray;
	for (int32 i = 0; i < 9; i++)
	{
		UPayloadBuilder::AddArrayItem(addressArray, UMirageUtility::GetWalletAddress());
	}

	const TArray<FString> items = { BlueHatAddress, RedHatAddress, WhiteHatAddress, BlueShoesAddress, RedShoesAddress, WhiteShoesAddress, BlueGlassesAddress, RedGlassesAddress, WhiteGlassesAddress };
	
	TArray<TSharedPtr<FJsonValue>> itemsArray;
	for (int32 i = 0; i < items.Num(); i++)
	{
		UPayloadBuilder::AddArrayItem(itemsArray, items[i]);
	}

	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddNestedArray(args, addressArray);
	UPayloadBuilder::AddNestedArray(args, itemsArray);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",	    UMirageUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameItemContractAddress);
	builder->SetStringField("abi_hash",	    abi_hash);
	builder->SetStringField("method",	    "balanceOfBatch");
	builder->SetArrayField("args",		    args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UMirageUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FMirageCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetItemsBalance - response: %s"), *content);

			FString data = jsonObject->GetStringField("data");

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}

```

<p align="right">(<a href="#top">back to top</a>)</p>

## 🗒 Current ERC Proposals

We have two ERC proposals.  
[ERC-4884  Rentable NFT Standard](https://github.com/Ankr-network/game-smart-contract-example/blob/master/ERC/rentable-nft.md)
[ERC-4911  Composability Extension For ERC-721 Standard](https://github.com/Ankr-network/game-smart-contract-example/blob/master/ERC/composable-nft.md)


<p align="right">(<a href="#top">back to top</a>)</p>




