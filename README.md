
<h3 align="center">Unreal SDK</h3>

  <p align="center">
    The Unreal Engine is aimed at Developers wanting to implement blockchain features into their game. The Unreal Engine allows the game to connect and interact with the blockchain.
    <br />
    <!-- <a href="https://github.com/github_username/repo_name"></a> -->
    <br />
    <br />
    <a href="https://github.com/Ankr-network/game-unity-demo">View Demo</a>
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
    <li><a href="#01-connect-wallet">Connect Wallet</a></li>
  </ol>
</details>

<!-- GETTING STARTED -->

## 👉🏻 Getting Started

The Unreal Engine gets the back end client to connect and interact with the blockchain to send and retrieve data.

The baseURL is: http://45.77.189.28:5000/

- Front End - **Unity** 
- Back End 
    - **Ankr** handles all communication with the backend and blockchain via a baseURL
    - The **Unreal Engine** sends call functions and parameters and receives JSON responses as to whether the call is successful or not.

Currently, the Unreal Engine serves three Use Cases.

### 💫 Use Cases

1. Connect Wallet (MetaMask) and Authenticate User
2. Update NFT by signing and sending Transactions

### 🧰 Prerequisites

1. Smart Contracts must be deployed on the blockchain.  
2. Smart Contract addresses and ABI
2. Web3.js or Ethers.js

## 👝 01 Connect Wallet

1. Upon initialization, the client is called and a `device_id` is created.  

```js
deviceId = load->UniqueId;
baseUrl = "http://45.77.189.28:5000/";
```

2. The Connection status is queried and a `sessionId`is assigned subject to the Login Status.

```js
GetClient(FMirageConnectionStatus Status)
```

```js
FString recievedUri = JsonObject->GetStringField("uri");
				FString sessionId = JsonObject->GetStringField("session");
				bool needLogin = JsonObject->GetBoolField("login");
				session = sessionId;
```

2. Login via MetaMask is required to authenticate the session. A connection request is sent with the `device_id` as a parameter.

```js
FString url = baseUrl + "connect";
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, url);

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString("{\"device_id\": \"" + deviceId + "\"}");
	Request->ProcessRequest();
	return true;
```

3. If successful, a JSON object is returned with the `session_id` and `URI`. This is now a valid session and any transaction to the blockchain can be performed.  

<p align="right">(<a href="#top">back to top</a>)</p>

## 🚀 02 Performing Updates to NFTs 

Making updates to the NFT e.g. adding a red hat to a character requires signing and sending a transaction.

### Signing Transactions

Transactions are signed via a prompt from MetaMask requiring a signature.

### Sending Transactions

Any function in the contract can be called. The following are required.

* Contract Address
* ABI 
* Method
 
If you want to call any standard function on the smart contract, use the function `SendTransaction` to get all available functions via the client.

When a call is made, the back end client sends a `Ticket` via MetaMask if the call was successful.

### Checking Status of Ticket

Check status of tickets by calling `GetTicketResult`.

For example, if the game is hanging or you are still waiting.

Returns the status of the result.

Successful or unsuccessful.

### GetData Function

Use `GetData` function to call non standard functions.

```js
GetData(FString contract, FString abi, FString method, FString args, FMirageDelegate Result)
{
```

Calls are made using the following general format:

```js
FString url = baseUrl + "call/method";
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
```

### SendABI Function

If you are using non-standard functions you must use the `SendABI` function.

First, get the ABI from the smart contract, then send it. 

```js
SendABI(FString abi, FMirageDelegate Result)
```

The Call is as follows: 

```js
FString url = baseUrl + "SendABI";

```

### SignMessage Function

This can be used to sign any data message e.g. minting.
MetaMask requests a signature, then returns a ticket.

```js
SignMessage(FString message, FMirageDelegate Result)
```

A `ticketid` is returned. 

The Call is as follows:

```js
FString url = baseUrl + "sign/message";
```

### GetSignature Function

Send the `ticketid` returned in the previous function to get a signature.

```js
GetSignature(FString ticket, FMirageDelegate Result)
```

The following object is returned:

```js
{
	TSharedPtr<FJsonObject> data = JsonObject->GetObjectField("data");
	UE_LOG(LogTemp, Warning, TEXT("signature: %s"), *data->GetStringField("signature"));
	Result.ExecuteIfBound(data->GetStringField("signature"));
}
```
The Call is as follows:

```js
FString url = baseUrl + "result";
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-MirageSDK-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
```

### VerifySignature Function

This function compares the `SignMessage` to the `Signature` from the ticket. 

```js
VerifyMessage(FString message, FString signature, FMirageDelegate Result)
```

The Call is as follows:

```js
FString url = baseUrl + "verify/message";
```


<p align="right">(<a href="#top">back to top</a>)</p>

## 🗒 Current ERC Proposals

We have two ERC proposals.  
[ERC-4884  Rentable NFT Standard](https://github.com/Ankr-network/game-smart-contract-example/blob/master/ERC/rentable-nft.md)
[ERC-4911  Composability Extension For ERC-721 Standard](https://github.com/Ankr-network/game-smart-contract-example/blob/master/ERC/composable-nft.md)


<p align="right">(<a href="#top">back to top</a>)</p>




