
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
6. There are two ways you can create an instance of AnkrClient object which are shown below:

(6a) Open any of your blueprint, call "Construct Object From Class", select AnkrClient and store the reference in a variable.

<img width="517" alt="InstanceCreation" src="https://user-images.githubusercontent.com/99165088/189541606-8049b5c2-bfba-42df-989d-7bb424d9389c.png">

(6b) Locate your GameInstance.h if already created. If not Add C++ class from Content Browser in Unreal Engine, check 'Show All Classes' and select GameInstance. Name your class 'MyGameInstance'.

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

ConnectWallet is used to connect to the wallet app on your mobile device, on desktop a QR Code will be generated at the time the login button is pressed.
The session is saved to a variable for later use.

To Connect wallet call the C++ ConnectWallet in Blueprint shown below:

```js

void UAnkrClient::ConnectWallet(const FAnkrCallCompleteDynamicDelegate& Result);

```
<img width="494" alt="ConnectWalletpng" src="https://user-images.githubusercontent.com/99165088/187159950-fc5d030a-012d-42cb-bdfa-3383867f48b3.png">

2. If Login is required, wallet app will be opened on your mobile device to connect to.

3. If already logged in then GetWalletInfo is used to the get the wallet information.

To Get wallet information call the C++ GetWalletInfo in Blueprint shown below:

```js

void UAnkrClient::GetWalletInfo(const FAnkrCallCompleteDynamicDelegate& Result);

```
<img width="529" alt="GetWalletpng" src="https://user-images.githubusercontent.com/99165088/187161391-da60ae1c-38a5-47e9-9553-94a5a6349a2a.png">

<p align="right">(<a href="#top">back to top</a>)</p>

### Sending Transactions

SendTransaction is used to send a transaction and a ticket will be generated.
Metamask will show popup to confirm the ticket generated by the transaction.

```js

const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");

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

const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + contract + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + args + "\"}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - CallMethod - response: %s"), *content);

	callback.ExecuteIfBound(content, content, "", -1, false);

}, Result, false);

```

### Send ABI

SendABI is used to get the hash of the abi. 

```js

const TCHAR* find = TEXT("\"");
const TCHAR* replace = TEXT("\\\"");
FString payload = FString("{\"abi\": \"" + abi.Replace(find, replace, ESearchCase::IgnoreCase) + "\"}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_ABI, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - SendABI - response: %s"), *content);

	FString abi = jsonObject->GetStringField("abi");
			
	callback.ExecuteIfBound(content, abi, "", -1, false);
			
}, Result, false);

```

### Sign Message

SignMessage is used to sign a message
Metamask will show popup to sign or cancel the signing of the message.

```js

const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"message\":\"" + message + "\"}");

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

```

### Get Signature

GetSignature is used to get the result whether the user has signed the message or cancelled the signing.

```js

const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"ticket\":\"" + ticket + "\"}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_RESULT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - GetSignature - response: %s"), *content);

	TSharedPtr<FJsonObject> data = jsonObject->GetObjectField("data");
	FString signature			 = data->GetStringField("signature");

	callback.ExecuteIfBound(content, signature, "", -1, false);

}, Result, false);

```

### Verify Signature

VerifyMessage is used to verify if the connected user has signed the message.

```js

const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"message\":\"" + message + "\", \"signature\":\"" + signature + "\"}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_VERIFY_MESSAGE, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("AnkrClient - VerifyMessage - response: %s"), *content);

	FString address = jsonObject->GetStringField("address");

	callback.ExecuteIfBound(content, address, "", -1, false);

}, Result, false);

```

## 🚀 02 Performing Updates to NFTs 

Getting NFTs and Making updates to the NFT.

### Get NFT Info

GetNFTInfo is used to get the NFT information.

```js

FString method = "getTokenDetails";
const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + ContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": \"" + FString::FromInt(tokenId) + "\"}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - GetNFTInfo - response: %s"), *content);

	callback.ExecuteIfBound(content, content, "", -1, false);

}, Result, false);

```

### Update NFT

UpdateNFT is used to update the information on the NFT.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

FRequestBodyStruct body{};
body.device_id = UAnkrUtility::GetDeviceID();
body.contract_address = ContractAddress;
body.abi_hash = abi_hash;
body.method = "updateTokenWithSignedMessage";
body.args.Add(_item);

const FString payload = FString(FRequestBodyStruct::ToJson(body));

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - UpdateNFT - response: %s"), *content);

	FString ticket = jsonObject->GetStringField("ticket");

	callback.ExecuteIfBound(content, ticket, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
	UAnkrUtility::SetLastRequest("UpdateNFT");
	FPlatformProcess::LaunchURL(UAnkrUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

}, Result, true);

```

## 🚀 03 Wearables NFT

### Mint Character

MintCharacter is used to mint a character to the wallet address.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

FString method = "safeMint";
const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": [\"" + to + "\"]}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - MintCharacter - response: %s"), *content);

	FString ticket = jsonObject->GetStringField("ticket");

	UAnkrUtility::SetLastRequest("MintCharacter");
	callback.ExecuteIfBound(content, ticket, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
	FPlatformProcess::LaunchURL(UAnkrUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

}, Result, true);

```

### Mint Items

MintItems is used to mint items in a batch.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

FString method = "mintBatch";
FString args = "[\"" + to + "\", [\"" + BlueHatAddress + "\", \"" + RedHatAddress + "\", \"" + BlueShoesAddress + "\", \"" + WhiteShoesAddress + "\", \"" + RedGlassesAddress + "\", \"" + WhiteGlassesAddress + "\"], [1, 2, 3, 4, 5, 6], \"0x\"]";
	args = args.Replace(TEXT(" "), TEXT(""));
const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + GameItemContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": " + args + "}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - MintItems - response: %s"), *content);

	FString ticket = jsonObject->GetStringField("ticket");

	UAnkrUtility::SetLastRequest("MintItems");
	callback.ExecuteIfBound(content, ticket, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
	FPlatformProcess::LaunchURL(UAnkrUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

}, Result, true);

```

### Game Item Set Approval

GameItemSetApproval is used to set an approval for the call operator.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

FString method = "setApprovalForAll";
const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + GameItemContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": [\"" + GameCharacterContractAddress + "\", true ]}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GameItemSetApproval - response: %s"), *content);

	FString data = content;
	bool result = jsonObject->GetBoolField("result");
	if (result)
	{
		FString ticket = jsonObject->GetStringField("ticket");
		data = ticket;
	}

	UAnkrUtility::SetLastRequest("GameItemSetApproval");
	callback.ExecuteIfBound(content, data, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
	FPlatformProcess::LaunchURL(UAnkrUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

}, Result, true);

```

### Get Character Balance

GetCharacterBalance is used to get the token balance that the user has in their wallet.
The 'data' shows the number of tokens that the user holds.

```js

FString method = "balanceOf";
const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": [\"" + address + "\"]}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterBalance - response: %s"), *content);

	FString data = jsonObject->GetStringField("data");

	callback.ExecuteIfBound(content, data, "", -1, false);

}, Result, false);

```

### Get Character Token Id

GetCharacterTokenId is used to get the token Id at the specified index.
The 'data' shows the id of the character.

```js

FString method = "tokenOfOwnerByIndex";
const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": [\"" + owner + "\", \"" + index + "\"]}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterTokenId - response: %s"), *content);

	FString data = jsonObject->GetStringField("data");

	callback.ExecuteIfBound(content, data, "", -1, false);

}, Result, false);

```

### Change Hat

ChangeHat is used to change the hat of the character to another available hat.
Metamask will show popup to sign or confirm the transaction for that ticket.

```js

FString method = "changeHat";
const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": [\"" + FString::FromInt(characterId) + "\", \"" + hatAddress + "\"]}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this, hatAddress](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - ChangeHat - response: %s"), *content);

	FString ticket = jsonObject->GetStringField("ticket");

	callback.ExecuteIfBound(content, ticket, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
	FPlatformProcess::LaunchURL(UAnkrUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

}, Result, true);

```

### Get Hat

GetHat is used to get the current hat of the character.
The 'data' shows the token address that the player has.

```js

FString method = "getHat";
const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + GameCharacterContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": [\"" + FString::FromInt(characterId) + "\"]}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetHat - response: %s"), *content);

	FString data = jsonObject->GetStringField("data");

	callback.ExecuteIfBound(content, data, "", -1, false);

}, Result, false);

```

### Get Ticket Result

GetTicketResult is used to get the result of the ticket.
The 'status' shows whether the result for the ticket signed has a success with a transaction hash.
The 'code' shows a code number related to a specific failure or success.

```js

const FString payload = FString("{\"ticket\": \"" + ticketId + "\" }");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_RESULT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetWearableNFTResult - response: %s"), *content);

	bool result                    = jsonObject->GetBoolField("result");
	TSharedPtr<FJsonObject> object = jsonObject->GetObjectField("data");
	FString transactionHash        = object->GetStringField("tx_hash");
	FString status                 = object->GetStringField("status");
	UE_LOG(LogTemp, Warning, TEXT("tx_hash: %s | status: %s"), *transactionHash, *status);

	callback.ExecuteIfBound(content, content, "", -1, false);

}, Result, false);

```

### Get Items Balance

GetItemsBalance is used to get the balance of items in a batch.
The 'data' shows a response of an array of balances for each token, in the sequence that were sent as a request.

```js

FString method = "balanceOfBatch";
FString args = "[ [\"" + UAnkrUtility::GetWalletAddress() + "\", \"" + UAnkrUtility::GetWalletAddress() + "\", \"" + UAnkrUtility::GetWalletAddress() + "\", \"" + UAnkrUtility::GetWalletAddress() + "\", \"" + UAnkrUtility::GetWalletAddress() + "\", \"" + UAnkrUtility::GetWalletAddress() + "\", \"" + UAnkrUtility::GetWalletAddress() + "\", \"" + UAnkrUtility::GetWalletAddress() + "\", \"" + UAnkrUtility::GetWalletAddress() + "\"], [\"" + BlueHatAddress + "\", \"" + RedHatAddress + "\", \"" + WhiteHatAddress + "\", \"" + BlueShoesAddress + "\", \"" + RedShoesAddress + "\", \"" + WhiteShoesAddress + "\", \"" + BlueGlassesAddress + "\", \"" + RedGlassesAddress + "\", \"" + WhiteGlassesAddress + "\"]]";
const FString payload = FString("{\"device_id\": \"" + UAnkrUtility::GetDeviceID() + "\", \"contract_address\": \"" + GameItemContractAddress + "\", \"abi_hash\": \"" + abi_hash + "\", \"method\": \"" + method + "\", \"args\": " + args + "}");

SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetItemsBalance - response: %s"), *content);

	FString data = jsonObject->GetStringField("data");

	callback.ExecuteIfBound(content, data, "", -1, false);

}, Result, false);

```

<p align="right">(<a href="#top">back to top</a>)</p>

## 🗒 Current ERC Proposals

We have two ERC proposals.  
[ERC-4884  Rentable NFT Standard](https://github.com/Ankr-network/game-smart-contract-example/blob/master/ERC/rentable-nft.md)
[ERC-4911  Composability Extension For ERC-721 Standard](https://github.com/Ankr-network/game-smart-contract-example/blob/master/ERC/composable-nft.md)


<p align="right">(<a href="#top">back to top</a>)</p>




