#pragma once

#include "MirageClient.h"
#include "WearableNFTExample.generated.h"

const FString BlueHatAddress      = "0x00010000000000000000000000000000000000000000000000000000000001"; //6901746346790563787434755862277025452451108972170386555162524223799297
const FString RedHatAddress       = "0x00010000000000000000000000000000000000000000000000000000000002"; //6901746346790563787434755862277025452451108972170386555162524223799298
const FString WhiteHatAddress     = "0x00010000000000000000000000000000000000000000000000000000000003"; //6901746346790563787434755862277025452451108972170386555162524223799299
const FString BlueShoesAddress    = "0x00020000000000000000000000000000000000000000000000000000000001";
const FString RedShoesAddress     = "0x00020000000000000000000000000000000000000000000000000000000002";
const FString WhiteShoesAddress   = "0x00020000000000000000000000000000000000000000000000000000000003";
const FString BlueGlassesAddress  = "0x00030000000000000000000000000000000000000000000000000000000001";
const FString RedGlassesAddress   = "0x00030000000000000000000000000000000000000000000000000000000002";
const FString WhiteGlassesAddress = "0x00030000000000000000000000000000000000000000000000000000000003";

/// UWearableNFTExample provide various functions to mint character, mint items, get balance and changeHat etc.
UCLASS(Blueprintable, BlueprintType)
class MIRAGESDK_API UWearableNFTExample : public UMirageClient
{
	GENERATED_UCLASS_BODY()

public:

//#ifndef DOXYGEN_SHOULD_SKIP_THIS	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameItemContractAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameItemABI;
			 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameCharacterContractAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameCharacterABI;
			  
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString TransactionGasLimit;

	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK") FString GetBlueHatAddress();
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK") FString GetRedHatAddress();
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK") FString GetWhiteHatAddress();
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK") FString GetBlueShoesAddress();
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK") FString GetRedShoesAddress();
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK") FString GetWhiteShoesAddress();
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK") FString GetBlueGlassesAddress();
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK") FString GetRedGlassesAddress();
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK") FString GetWhiteGlassesAddress();
//#endif /* DOXYGEN_SHOULD_SKIP_THIS */

	/// MintItems function is used to mint a batch of items to the user and requires the user confirmation through wallet such as metamask.
	///
	/// The function requires parameters described below and returns nothing.
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device_id, contract_address, abi_hash, method and args. The format is described in the body section below.
	/// string data will be received in json response for a "ticket".
	///
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// @attention YOUR_ITEM_ADDRESS(s) and YOUR_ITEM_QUANTITY(s) represent each other respectively.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "contract_address":"0xD0eF33b38D8525728902D90b20d6e2F303B8dc2C", "abi_hash":"YOUR_ABI_HASH", "method":"mintBatch", "args":["TO_WALLET_ADDRESS", ["YOUR_ITEM_ADDRESS", "YOUR_ITEM_ADDRESS", "YOUR_ITEM_ADDRESS", "YOUR_ITEM_ADDRESS", "YOUR_ITEM_ADDRESS", "YOUR_ITEM_ADDRESS"], [YOUR_ITEM_QUANTITY, YOUR_ITEM_QUANTITY, YOUR_ITEM_QUANTITY, YOUR_ITEM_QUANTITY, YOUR_ITEM_QUANTITY, YOUR_ITEM_QUANTITY], \"0x\"]}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void MintItems(FString abi_hash, FString to, FMirageCallCompleteDynamicDelegate Result);

	/// MintCharacter function is used to mint a character to the user and requires the user confirmation through wallet such as metamask.
	///
	/// The function requires parameters described below and returns nothing.
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device_id, contract_address, abi_hash, method and args. The format is described in the body section below.
	/// string data will be received in json response for a "ticket".
	///
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param to The address of the user to which the character should be minted.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "contract_address":"0x7081F409F750EACD27867c988b4B3771d935Fe16", "abi_hash":"YOUR_ABI_HASH", "method":"safeMint", "args:["TO_WALLET_ADDRESS"]}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void MintCharacter(FString abi_hash, FString to, FMirageCallCompleteDynamicDelegate Result);

	/// GameItemSetApproval function is used to set an approval to mint items for the user and requires the user confirmation through wallet such as metamask.
	///
	/// The function requires parameters described below and returns nothing.
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device_id, contract_address, abi_hash, method and args. The format is described in the body section below.
	/// string data will be received in json response for a "ticket".
	///
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param callOperator The address of the contract that will operate the items.
	/// @param approved Whether the approval is granted or not.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "contract_address":"0xD0eF33b38D8525728902D90b20d6e2F303B8dc2C", "abi_hash":"YOUR_ABI_HASH", "method":"setApprovalForAll", "args:["0xD0eF33b38D8525728902D90b20d6e2F303B8dc2C", true]}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void GameItemSetApproval(FString abi_hash, FString callOperator, bool approved, FMirageCallCompleteDynamicDelegate Result);

	/// GetCharacterBalance function is used to get the token balance that user holds.
	///
	/// The function requires parameters described below and returns nothing.
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device_id, contract_address, abi_hash, method and args. The format is described in the body section below.
	/// string data will be received in json response for a "data".
	///
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param address The address of the user to check the balance against.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "contract_address":"0x7081F409F750EACD27867c988b4B3771d935Fe16", "abi_hash":"YOUR_ABI_HASH", "method":"balanceOf", "args":["WALLET_ADDRESS"]}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void GetCharacterBalance(FString abi_hash, FString address, FMirageCallCompleteDynamicDelegate Result);

	/// GetCharacterTokenId function is used to get the id of the token at the specified index.
	///
	/// The function requires parameters described below and returns nothing.
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device_id, contract_address, abi_hash, method and args. The format is described in the body section below.
	/// string data will be received in json response for a "ticket".
	///
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param tokenBalance The token balance obtained by GetCharacterBalance(FString, FString, FMirageCallCompleteDynamicDelegate);
	/// @param owner The address of the owner of the token.
	/// @param index The index of the token.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "contract_address":"0x7081F409F750EACD27867c988b4B3771d935Fe16", "abi_hash":"YOUR_ABI_HASH", "method":"tokenOfOwnerByIndex", "args":["WALLET_ADDRESS", "INDEX"]}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void GetCharacterTokenId(FString abi_hash, int tokenBalance, FString owner, FString index, FMirageCallCompleteDynamicDelegate Result);

	/// ChangeHat function is used to change the hat of the character and requires the user confirmation through wallet such as metamask.
	///
	/// The function requires parameters described below and returns nothing.
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a device_id, contract_address, abi_hash, method and args. The format is described in the body section below.
	/// string data will be received in json response for a "ticket".
	///
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param characterId The character id obtained by GetCharacterTokenId(FString, int, FString, FString, FMirageCallCompleteDynamicDelegate);
	/// @param hatAddress The address of the hat to change hat to.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "contract_address":"0x7081F409F750EACD27867c988b4B3771d935Fe16", "abi_hash":"YOUR_ABI_HASH", "method":"changeHat", "args":["TOKEN_ID", "HAT_ADDRESS"]}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void ChangeHat(FString abi_hash, int characterId, FString hatAddress, FMirageCallCompleteDynamicDelegate Result);

	/// GetHat function is used to get the current hat of the character and requires the user confirmation through wallet such as metamask.
	///
	/// The function requires a parameter described below and returns nothing.
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a  device_id, contract_address, abi_hash, method and args. The format is described in the body section below.
	/// string data will be received in json response for a "data".
	///
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param characterId The character id obtained by GetCharacterTokenId(FString, int, FString, FString, FMirageCallCompleteDynamicDelegate);
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "contract_address":"0x7081F409F750EACD27867c988b4B3771d935Fe16", "abi_hash":"YOUR_ABI_HASH", "method":"getHat", "args":["TOKEN_ID"]}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void GetHat(FString abi_hash, int characterId, FMirageCallCompleteDynamicDelegate Result);
	
	/// GetTicketResult function is used to get the result of the ticket generated by MintItems(FString, FString, FMirageCallCompleteDynamicDelegate), MintCharacter(FString, FString, FMirageCallCompleteDynamicDelegate),
	/// GameItemSetApproval(FString, FString, bool, FMirageCallCompleteDynamicDelegate) or ChangeHat(FString, int, bool, FString, FMirageCallCompleteDynamicDelegate)
	///
	/// The function requires a parameter described below and returns nothing.
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a data and txHash. The format is described in the body section below.
	/// string data will be received in json response for a "data".
	///
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"ticket":"YOUR_TICKET"}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void GetWearableNFTResult(FString ticketId, FMirageCallCompleteDynamicDelegate Result);

	/// GetItemsBalance function is used to get the balance of items in batch.
	///
	/// The function requires a parameter described below and returns nothing.
	/// Inside the function, A POST request is sent to the Mirage API. The request needs a json body containing a ticket. The format is described in the body section below.
	/// string data will be received in json response for a "data".
	///
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param address The address of the user to check the balance of items against.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	/// 
	/// ### Body
	/// ~~~~~~~~~~~~~~~~~~~~~~~.cpp
	/// {"device_id":"YOUR_DEVICE_ID", "contract_address":"0xD0eF33b38D8525728902D90b20d6e2F303B8dc2C", "abi_hash":"YOUR_ABI_HASH", "method":"balanceOfBatch", "args:[ ["activeAccount", " + activeAccount", "activeAccount", "activeAccount", "activeAccount", "activeAccount", "activeAccount", "activeAccount", "activeAccount"], ["BlueHatAddress", "RedHatAddress ", "WhiteHatAddress ", "BlueShoesAddress", "RedShoesAddress", "WhiteShoesAddress", "BlueGlassesAddress", "RedGlassesAddress", "WhiteGlassesAddress"]]}
	/// ~~~~~~~~~~~~~~~~~~~~~~~
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void GetItemsBalance(FString abi_hash, FString address, FMirageCallCompleteDynamicDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	int GetItemValueFromBalances(FString data, int index);

	/// GetTokenURI function is used to get token uri of an NFT.
	///
	/// The function requires a parameter described below and returns nothing.
	/// Inside the function, A POST request is sent to the Mirage. The request needs a json body containing a ticket. The format is described in the body section below.
	/// string data will be received in json response for a "data".
	///
	/// @param abi_hash The hash of the abi string of the contract.
	/// @param tokenId The id the token hold by the user.
	/// @param Result A callback delegate that will be triggered once a response is received with data.
	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	void GetTokenURI(FString abi_hash, int tokenId, FMirageCallCompleteDynamicDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MIRAGE SDK")
	FString GetHatAddressByID(FString _hatID);
};