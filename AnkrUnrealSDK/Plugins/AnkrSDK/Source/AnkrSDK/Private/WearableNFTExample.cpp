#include "WearableNFTExample.h"
#include "AnkrUtility.h"
#include "PayloadBuilder.h"

UWearableNFTExample::UWearableNFTExample(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameItemContractAddress		 = "0xD0eF33b38D8525728902D90b20d6e2F303B8dc2C";
	//GameItemContractAddress		 = "0x26aFc7805Aa279fB0E806c2bc1e2bF37A70F995d";
	GameItemABI					 = "[{\"inputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"constructor\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"},{\"indexed\":false,\"internalType\":\"bool\",\"name\":\"approved\",\"type\":\"bool\"}],\"name\":\"ApprovalForAll\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"previousAdminRole\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"newAdminRole\",\"type\":\"bytes32\"}],\"name\":\"RoleAdminChanged\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"sender\",\"type\":\"address\"}],\"name\":\"RoleGranted\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"sender\",\"type\":\"address\"}],\"name\":\"RoleRevoked\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"indexed\":false,\"internalType\":\"uint256[]\",\"name\":\"ids\",\"type\":\"uint256[]\"},{\"indexed\":false,\"internalType\":\"uint256[]\",\"name\":\"values\",\"type\":\"uint256[]\"}],\"name\":\"TransferBatch\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"id\",\"type\":\"uint256\"},{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"value\",\"type\":\"uint256\"}],\"name\":\"TransferSingle\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"string\",\"name\":\"value\",\"type\":\"string\"},{\"indexed\":true,\"internalType\":\"uint256\",\"name\":\"id\",\"type\":\"uint256\"}],\"name\":\"URI\",\"type\":\"event\"},{\"inputs\":[],\"name\":\"DEFAULT_ADMIN_ROLE\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"MINTER_ROLE\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"URI_SETTER_ROLE\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"id\",\"type\":\"uint256\"}],\"name\":\"balanceOf\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address[]\",\"name\":\"accounts\",\"type\":\"address[]\"},{\"internalType\":\"uint256[]\",\"name\":\"ids\",\"type\":\"uint256[]\"}],\"name\":\"balanceOfBatch\",\"outputs\":[{\"internalType\":\"uint256[]\",\"name\":\"\",\"type\":\"uint256[]\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"id\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"value\",\"type\":\"uint256\"}],\"name\":\"burn\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"internalType\":\"uint256[]\",\"name\":\"ids\",\"type\":\"uint256[]\"},{\"internalType\":\"uint256[]\",\"name\":\"values\",\"type\":\"uint256[]\"}],\"name\":\"burnBatch\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"id\",\"type\":\"uint256\"}],\"name\":\"exists\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"}],\"name\":\"getRoleAdmin\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"grantRole\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"hasRole\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"}],\"name\":\"isApprovedForAll\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"id\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"amount\",\"type\":\"uint256\"},{\"internalType\":\"bytes\",\"name\":\"data\",\"type\":\"bytes\"}],\"name\":\"mint\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256[]\",\"name\":\"ids\",\"type\":\"uint256[]\"},{\"internalType\":\"uint256[]\",\"name\":\"amounts\",\"type\":\"uint256[]\"},{\"internalType\":\"bytes\",\"name\":\"data\",\"type\":\"bytes\"}],\"name\":\"mintBatch\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"renounceRole\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"revokeRole\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256[]\",\"name\":\"ids\",\"type\":\"uint256[]\"},{\"internalType\":\"uint256[]\",\"name\":\"amounts\",\"type\":\"uint256[]\"},{\"internalType\":\"bytes\",\"name\":\"data\",\"type\":\"bytes\"}],\"name\":\"safeBatchTransferFrom\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"id\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"amount\",\"type\":\"uint256\"},{\"internalType\":\"bytes\",\"name\":\"data\",\"type\":\"bytes\"}],\"name\":\"safeTransferFrom\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"},{\"internalType\":\"bool\",\"name\":\"approved\",\"type\":\"bool\"}],\"name\":\"setApprovalForAll\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"string\",\"name\":\"newuri\",\"type\":\"string\"}],\"name\":\"setURI\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes4\",\"name\":\"interfaceId\",\"type\":\"bytes4\"}],\"name\":\"supportsInterface\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"id\",\"type\":\"uint256\"}],\"name\":\"totalSupply\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"name\":\"uri\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}],\"stateMutability\":\"view\",\"type\":\"function\"}]";

	GameCharacterContractAddress = "0x7081F409F750EACD27867c988b4B3771d935Fe16";
	//GameCharacterContractAddress = "0x2ff8d8a0e5d8e3cf34aa490abfd8f365e1f77f0d";
	//GameCharacterContractAddress = "0x10555B832DE7bAD8459d6de1D8E8F5ad990709A0";
	GameCharacterABI			 = "[{\"inputs\":[{\"internalType\":\"address\",\"name\":\"gameItemContractAddress\",\"type\":\"address\"}],\"stateMutability\":\"nonpayable\",\"type\":\"constructor\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"address\",\"name\":\"owner\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"approved\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"Approval\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"address\",\"name\":\"owner\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"},{\"indexed\":false,\"internalType\":\"bool\",\"name\":\"approved\",\"type\":\"bool\"}],\"name\":\"ApprovalForAll\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"characterId\",\"type\":\"uint256\"},{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"oldGlassesId\",\"type\":\"uint256\"},{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"newGlassesId\",\"type\":\"uint256\"}],\"name\":\"GlassesChanged\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"characterId\",\"type\":\"uint256\"},{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"oldHatId\",\"type\":\"uint256\"},{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"newHatId\",\"type\":\"uint256\"}],\"name\":\"HatChanged\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"previousAdminRole\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"newAdminRole\",\"type\":\"bytes32\"}],\"name\":\"RoleAdminChanged\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"sender\",\"type\":\"address\"}],\"name\":\"RoleGranted\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"sender\",\"type\":\"address\"}],\"name\":\"RoleRevoked\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"characterId\",\"type\":\"uint256\"},{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"oldShoesId\",\"type\":\"uint256\"},{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"newShoesId\",\"type\":\"uint256\"}],\"name\":\"ShoesChanged\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"Transfer\",\"type\":\"event\"},{\"inputs\":[],\"name\":\"DEFAULT_ADMIN_ROLE\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"MINTER_ROLE\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"approve\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"owner\",\"type\":\"address\"}],\"name\":\"balanceOf\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"burn\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"characterId\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"newGlassesId\",\"type\":\"uint256\"}],\"name\":\"changeGlasses\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"characterId\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"newHatId\",\"type\":\"uint256\"}],\"name\":\"changeHat\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"characterId\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"newShoesId\",\"type\":\"uint256\"}],\"name\":\"changeShoes\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"getApproved\",\"outputs\":[{\"internalType\":\"address\",\"name\":\"\",\"type\":\"address\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"characterId\",\"type\":\"uint256\"}],\"name\":\"getGlasses\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"characterId\",\"type\":\"uint256\"}],\"name\":\"getHat\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"}],\"name\":\"getRoleAdmin\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"characterId\",\"type\":\"uint256\"}],\"name\":\"getShoes\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"grantRole\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"hasRole\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"owner\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"}],\"name\":\"isApprovedForAll\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"name\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"\",\"type\":\"address\"},{\"internalType\":\"uint256[]\",\"name\":\"\",\"type\":\"uint256[]\"},{\"internalType\":\"uint256[]\",\"name\":\"\",\"type\":\"uint256[]\"},{\"internalType\":\"bytes\",\"name\":\"\",\"type\":\"bytes\"}],\"name\":\"onERC1155BatchReceived\",\"outputs\":[{\"internalType\":\"bytes4\",\"name\":\"\",\"type\":\"bytes4\"}],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"},{\"internalType\":\"bytes\",\"name\":\"\",\"type\":\"bytes\"}],\"name\":\"onERC1155Received\",\"outputs\":[{\"internalType\":\"bytes4\",\"name\":\"\",\"type\":\"bytes4\"}],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"ownerOf\",\"outputs\":[{\"internalType\":\"address\",\"name\":\"\",\"type\":\"address\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"renounceRole\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"revokeRole\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"}],\"name\":\"safeMint\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"safeTransferFrom\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"},{\"internalType\":\"bytes\",\"name\":\"_data\",\"type\":\"bytes\"}],\"name\":\"safeTransferFrom\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"},{\"internalType\":\"bool\",\"name\":\"approved\",\"type\":\"bool\"}],\"name\":\"setApprovalForAll\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes4\",\"name\":\"interfaceId\",\"type\":\"bytes4\"}],\"name\":\"supportsInterface\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"symbol\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"index\",\"type\":\"uint256\"}],\"name\":\"tokenByIndex\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"owner\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"index\",\"type\":\"uint256\"}],\"name\":\"tokenOfOwnerByIndex\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"tokenURI\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"totalSupply\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"transferFrom\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"}]";

	TransactionGasLimit			 = "1000000";
}

FString UWearableNFTExample::GetBlueHatAddress()      { return BlueHatAddress; }
FString UWearableNFTExample::GetRedHatAddress()       { return RedHatAddress; }
FString UWearableNFTExample::GetWhiteHatAddress()     { return WhiteHatAddress; }
FString UWearableNFTExample::GetBlueShoesAddress()    { return BlueShoesAddress; }
FString UWearableNFTExample::GetRedShoesAddress()     { return RedShoesAddress; }
FString UWearableNFTExample::GetWhiteShoesAddress()   { return WhiteShoesAddress; }
FString UWearableNFTExample::GetBlueGlassesAddress()  { return BlueGlassesAddress; }
FString UWearableNFTExample::GetRedGlassesAddress()   { return RedGlassesAddress; }
FString UWearableNFTExample::GetWhiteGlassesAddress() { return WhiteGlassesAddress; }

void UWearableNFTExample::MintItems(FString abi_hash, FString to, FAnkrCallCompleteDynamicDelegate Result)
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
	builder->SetStringField("device_id",        UAnkrUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameItemContractAddress);
	builder->SetStringField("abi_hash",			abi_hash);
	builder->SetStringField("method",			"mintBatch");
	builder->SetArrayField ("args",				args);
	const FString payload = UPayloadBuilder::Build(builder);

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
}

void UWearableNFTExample::MintCharacter(FString abi_hash, FString to, FAnkrCallCompleteDynamicDelegate Result)
{
	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, to);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",		UAnkrUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameCharacterContractAddress);
	builder->SetStringField("abi_hash",			abi_hash);
	builder->SetStringField("method",			"safeMint");
	builder->SetArrayField ("args",				args);
	const FString payload = UPayloadBuilder::Build(builder);

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
}

void UWearableNFTExample::GameItemSetApproval(FString abi_hash, FString callOperator, bool approved, FAnkrCallCompleteDynamicDelegate Result)
{
	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, GameCharacterContractAddress);
	UPayloadBuilder::AddArrayItem(args, true);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",		UAnkrUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameItemContractAddress);
	builder->SetStringField("abi_hash",			abi_hash);
	builder->SetStringField("method",			"setApprovalForAll");
	builder->SetArrayField("args",				args);
	const FString payload = UPayloadBuilder::Build(builder);

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
}

void UWearableNFTExample::GetCharacterBalance(FString abi_hash, FString address, FAnkrCallCompleteDynamicDelegate Result)
{
	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, address);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",		UAnkrUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameCharacterContractAddress);
	builder->SetStringField("abi_hash",			abi_hash);
	builder->SetStringField("method",			"balanceOf");
	builder->SetArrayField("args",				args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterBalance - response: %s"), *content);

			FString data = jsonObject->GetStringField("data");

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}

void UWearableNFTExample::GetCharacterTokenId(FString abi_hash, int tokenBalance, FString owner, FString index, FAnkrCallCompleteDynamicDelegate Result)
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
	builder->SetStringField("device_id",		UAnkrUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameCharacterContractAddress);
	builder->SetStringField("abi_hash",			abi_hash);
	builder->SetStringField("method",			"tokenOfOwnerByIndex");
	builder->SetArrayField("args",				args);
	const FString payload = UPayloadBuilder::Build(builder);

	UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterTokenId - payload: %s"), *payload);

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetCharacterTokenId - response: %s"), *content);

			FString data = jsonObject->GetStringField("data");

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}

void UWearableNFTExample::ChangeHat(FString abi_hash, int characterId, bool hasHat, FString hatAddress, FAnkrCallCompleteDynamicDelegate Result)
{
	if (!hasHat || characterId == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - ChangeHat - CharacterID or HatID is null"));
		return;
	}

	if (hatAddress.Equals(BlueHatAddress)) UAnkrUtility::SetLastRequest("ChangeHatBlue");
	else if (hatAddress.Equals(RedHatAddress)) UAnkrUtility::SetLastRequest("ChangeHatRed");

	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, FString::FromInt(characterId));
	UPayloadBuilder::AddArrayItem(args, hatAddress);

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",		UAnkrUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameCharacterContractAddress);
	builder->SetStringField("abi_hash",			abi_hash);
	builder->SetStringField("method",			"changeHat");
	builder->SetArrayField("args",				args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_SEND_TRANSACTION, "POST", payload, [this, hatAddress](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - ChangeHat - response: %s"), *content);

			FString ticket = jsonObject->GetStringField("ticket");

			callback.ExecuteIfBound(content, ticket, "", -1, false);

#if PLATFORM_ANDROID || PLATFORM_IOS
			FPlatformProcess::LaunchURL(UAnkrUtility::GetSession().GetCharArray().GetData(), NULL, NULL);
#endif

		}, Result, true);
}

void UWearableNFTExample::GetHat(FString abi_hash, int characterId, FAnkrCallCompleteDynamicDelegate Result)
{
	TArray<TSharedPtr<FJsonValue>> args;
	UPayloadBuilder::AddArrayItem(args, FString::FromInt(characterId));

	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("device_id",		UAnkrUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameCharacterContractAddress);
	builder->SetStringField("abi_hash",			abi_hash);
	builder->SetStringField("method",			"getHat");
	builder->SetArrayField("args",				args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetHat - response: %s"), *content);

			FString data = jsonObject->GetStringField("data");

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}

void UWearableNFTExample::GetWearableNFTResult(FString ticketId, FAnkrCallCompleteDynamicDelegate Result)
{
	const FString payload = UPayloadBuilder::BuildPayload("ticket", ticketId);

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_RESULT, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetWearableNFTResult - response: %s"), *content);

			FString data = content;
			int code = 1;

			if (UAnkrUtility::GetLastRequest().Equals("ChangeHatBlue") || UAnkrUtility::GetLastRequest().Equals("ChangeHatRed"))
			{
				bool result                    = jsonObject->GetBoolField("result");
				TSharedPtr<FJsonObject> object = jsonObject->GetObjectField("data");
				FString transactionHash        = object->GetStringField("tx_hash");
				FString status                 = object->GetStringField("status");
				UE_LOG(LogTemp, Warning, TEXT("tx_hash: %s | status: %s"), *transactionHash, *status);

				if (result && status == "success")
				{
					code = 123;
				}
			}

			callback.ExecuteIfBound(content, data, "", code, false);

		}, Result, false);
}

void UWearableNFTExample::GetItemsBalance(FString abi_hash, FString address, FAnkrCallCompleteDynamicDelegate Result)
{
	TArray<TSharedPtr<FJsonValue>> addressArray;
	for (int32 i = 0; i < 9; i++)
	{
		UPayloadBuilder::AddArrayItem(addressArray, UAnkrUtility::GetWalletAddress());
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
	builder->SetStringField("device_id",		UAnkrUtility::GetDeviceID());
	builder->SetStringField("contract_address", GameItemContractAddress);
	builder->SetStringField("abi_hash",			abi_hash);
	builder->SetStringField("method",			"balanceOfBatch");
	builder->SetArrayField("args",				args);
	const FString payload = UPayloadBuilder::Build(builder);

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetItemsBalance - response: %s"), *content);

			FString data = jsonObject->GetStringField("data");

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}

int UWearableNFTExample::GetItemValueFromBalances(FString data, int index)
{
	TArray<FString> tokens;
	FString seperator(",");
	data.ParseIntoArray(tokens, *seperator, true);
	int numberOfTokens = tokens.Num();

	if (numberOfTokens <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetItemValueFromBalances - numberOfTokens: %d"), numberOfTokens);
		return -1;
	}

	if (index > numberOfTokens - 1) index = numberOfTokens - 1;
	else if (index < -1) index = 0;


	return FCString::Atoi(*tokens[index]);
}

void UWearableNFTExample::GetTokenURI(FString abi_hash, int tokenId, FAnkrCallCompleteDynamicDelegate Result)
{
	const FString payload = UPayloadBuilder::BuildPayload("device_id",		  UPayloadBuilder::FStringToJsonValue(UAnkrUtility::GetDeviceID()),
														  "contract_address", UPayloadBuilder::FStringToJsonValue(GameCharacterContractAddress),
														  "abi_hash",		  UPayloadBuilder::FStringToJsonValue(abi_hash),
														  "method",			  UPayloadBuilder::FStringToJsonValue("tokenURI"),
														  "args",			  UPayloadBuilder::FStringToJsonValue(FString::FromInt(tokenId)));

	SendRequest(UAnkrUtility::GetUrl() + ENDPOINT_CALL_METHOD, "POST", payload, [this](const TArray<uint8> bytes, const FString content, const FAnkrCallCompleteDynamicDelegate& callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("WearableNFTExample - GetTokenURI - response: %s"), *content);

			FString data = jsonObject->GetStringField("data");

			callback.ExecuteIfBound(content, data, "", -1, false);

		}, Result, false);
}