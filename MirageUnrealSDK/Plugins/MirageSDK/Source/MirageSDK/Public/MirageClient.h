#pragma once

#include "MirageClientBase.h"
#include "MirageClient.generated.h"

#define DOXYGEN_SHOULD_SKIP_THIS

/// MirageClient provides various functions that are used to connect wallet and interact with the blockchain.
UCLASS(Blueprintable, BlueprintType)
class MIRAGESDK_API UMirageClient : public UMirageClientBase
{
	GENERATED_UCLASS_BODY()

public:

//#ifndef DOXYGEN_SHOULD_SKIP_THIS
	bool isDevelopment;
//#endif /* DOXYGEN_SHOULD_SKIP_THIS */

	/// Initialize function is used to initialize the MirageClient.
	///
	/// The function does not require a parameter and returns nothing.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void Initialize();

	/// Ping function is used to check if the Mirage API is alive.
	///
	/// The function requires a parameter described below and returns nothing.
	/// Inside the function, A GET request is sent to the Mirage API.
	/// string data will be received in json response with "pong".
	///
	/// @param Result A callback delegate that will be triggered once a response is received with a data.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void Ping(const FMirageCallCompleteDynamicDelegate& Result);

	/// ConnectWallet function is used to connect the wallet such as metamask etc.
	///
	/// The function requires a parameter described below and returns nothing.\n
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device id and the format is described in the body section below.\n
	/// string data will be received in json response for "login", "session" and "uri".\n
	/// "uri" will be used to launch metamask to confirm the user to connect wallet. Once connected you can call GetWalletInfo(const FMirageCallCompleteDynamicDelegate&) to get the wallet address.
	///
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	///
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void ConnectWallet(const FMirageCallCompleteDynamicDelegate& Result);

	/// GetWalletInfo function is used to get the wallet address and chain id.
	///
	/// The function requires a parameter described below and returns nothing.\n
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device id and the format is described in the body section below.\n
	/// string data will be received in json response for "accounts" and "chainId".
	///
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	///
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void GetWalletInfo(const FMirageCallCompleteDynamicDelegate& Result);

	/// SendABI function is used to get the hash of the abi string.
	///
	/// The function requires a parameter described below and returns nothing.\n
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing an abi string and the format is described in the body section below.\n
	/// string data will be received in json response for "abi".
	///
	/// @param abi An ABI string of a contract.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	///
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"abi":"YOUR_ABI"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void SendABI(FString abi, const FMirageCallCompleteDynamicDelegate& Result);


	/// SendTransaction function is used to send a transaction and requires the user confirmation through wallet such as metamask.
	///
	/// The function requires parameters described below and returns nothing.\n
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device_id, contract_address, abi_hash, method and args. The format is described in the body section below.\n
	/// string data will be received in json response for a "ticket".
	///
	/// @param contract The address of the contract to which you want to interact.
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param method The method that is to be called from the contract.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "contract_address":"YOUR_CONTRACT_ADDRESS", "abi_hash":"YOUR_ABI_HASH", "method":"YOUR_METHOD", "args:"YOUR_ARGS"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void SendTransaction(FString contract, FString abi_hash, FString method, FString args, const FMirageCallCompleteDynamicDelegate& Result);

	/// GetTicketResult function is used to get the result of the ticket generated by SendTransaction(FString, FString, FString, FString, const FMirageCallCompleteDynamicDelegate&).
	///
	/// The function requires a parameter described below and returns nothing.\n
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a ticket. The format is described in the body section below.\n
	/// string data will be received in json response for a "data".
	///
	/// @param ticketId The ticket generated by any writable function.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"ticket":"YOUR_TICKET"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	virtual void GetTicketResult(FString ticketId, const FMirageCallCompleteDynamicDelegate& Result);

	/// CallMethod function is used to get a data from blockchain and doesn't require the user confirmation through wallet such as metamask.
	///
	/// The function requires parameters described below and returns nothing.\n
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device_id, contract_address, abi_hash, method and args. The format is described in the body section below.\n
	/// string data will be received in json response for a "data".
	///
	/// @param contract The address of the contract to which you want to interact.
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param method The method that is to be called from the contract.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "contract_address":"YOUR_CONTRACT_ADDRESS", "abi_hash":"YOUR_ABI_HASH", "method":"YOUR_METHOD", "args:"YOUR_ARGS"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void CallMethod(FString contract, FString abi, FString method, FString args, const FMirageCallCompleteDynamicDelegate& Result);

	/// SignMessage function is used to sign a message and requires the user confirmation to sign through wallet such as metamask..
	///
	/// The function requires parameters described below and returns nothing.\n
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device_id and message. The format is described in the body section below.\n
	/// string data will be received in json response for a "data".
	///
	/// @param message The string message to be signed
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "message":"YOUR_MESSAGE"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void SignMessage(FString message, const FMirageCallCompleteDynamicDelegate& Result);

	/// GetSignature function is used to get the signature signed by the SignMessage(FString, const FMirageCallCompleteDynamicDelegate&) function.
	///
	/// The function requires a parameter described below and returns nothing.\n
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a ticket. The format is described in the body section below.\n
	/// string data will be received in json response for a "signature".
	///
	/// @param ticket The ticket received from the SignMessage(FString, const FMirageCallCompleteDynamicDelegate&) function.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"ticket":"YOUR_TICKET"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void GetSignature(FString ticket, const FMirageCallCompleteDynamicDelegate& Result);

	/// VerifyMessage function is used to verify the message that was signed.
	///
	/// The function requires parameters described below and returns nothing.\n
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device_id, message and signature. The format is described in the body section below.\n
	/// string data will be received in json response for an "address".
	///
	/// @param message The string message to be signed
	/// @param signature The signature received by GetSignature(FString, const FMirageCallCompleteDynamicDelegate&);
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// @attention The "message" should be the one that was used in SignMessage(FString, const FMirageCallCompleteDynamicDelegate&).
	/// @attention The "signature" should be the one that was obtained in GetSignature(FString, const FMirageCallCompleteDynamicDelegate&).
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "message":"YOUR_MESSAGE", "signature":"YOUR_SIGNATURE"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
    void VerifyMessage(FString message, FString signature, const FMirageCallCompleteDynamicDelegate& Result);

	/// CollectStatistics function is used to collect the statistics.
	///
	/// The function requires parameters described below and returns nothing.\n
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing an app_id, device_id and public_address. The format is described in the body section below.\n
	/// string data will be received in json response as a result.
	///
	/// @param app_id The app specific app_id.
	/// @param device_id The device specific device id.
	/// @param public_address The address of the contract to which you want to interact.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"app_id":"YOUR_APP_ID", "device_id":"YOUR_DEVICE_ID", "public_address":"YOUR_PUBLIC_ADDRESS"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void CollectStatistics(FString _app_id, FString _device_id, FString _public_address);

	/// GetLastRequest function gets the name of last function that was called for the Mirage API. 
	///
	/// The function doesn't require a parameter and returns a string.\n
	/// Inside the function, the name of the last function that was called for the Mirage API is returned.
	///
	/// @returns The name of the last function that was called for Mirage API.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	FString GetLastRequest();

	/// SetLastRequest function sets the name of last function that is called for the Mirage API. 
	///
	/// The function requires a parameter described below and returns nothing.\n
	/// Inside the function, the name of the last function that is called for the Mirage API is set.
	///
	/// @param _lastRequest The name of the function that is called for the Mirage API.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void SetLastRequest(FString _lastRequest);
};