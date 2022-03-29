#include "UpdateNFTExample.h"
#include "ItemInfo.h"
#include "RequestBodyStructure.h"

UUpdateNFTExample::UUpdateNFTExample(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ContractAddress = "0x159D0A933137f3EC155f43834BDFCd534A8bfd61";
	ABI				= "[{\"inputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"constructor\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"address\",\"name\":\"owner\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"approved\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"Approval\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"address\",\"name\":\"owner\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"},{\"indexed\":false,\"internalType\":\"bool\",\"name\":\"approved\",\"type\":\"bool\"}],\"name\":\"ApprovalForAll\",\"type\":\"event\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"approve\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"burn\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"grantRole\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"components\":[{\"internalType\":\"uint256\",\"name\":\"itemType\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"strength\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"level\",\"type\":\"uint256\"}],\"internalType\":\"struct GameItem.Item\",\"name\":\"item\",\"type\":\"tuple\"}],\"name\":\"mintToken\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"components\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"itemType\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"strength\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"level\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"expireTime\",\"type\":\"uint256\"},{\"internalType\":\"bytes\",\"name\":\"signature\",\"type\":\"bytes\"}],\"internalType\":\"struct GameItem.ItemInfo\",\"name\":\"itemInfo\",\"type\":\"tuple\"}],\"name\":\"mintTokenWithSignedMessage\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"pause\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"Paused\",\"type\":\"event\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"renounceRole\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"revokeRole\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"previousAdminRole\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"newAdminRole\",\"type\":\"bytes32\"}],\"name\":\"RoleAdminChanged\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"sender\",\"type\":\"address\"}],\"name\":\"RoleGranted\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"sender\",\"type\":\"address\"}],\"name\":\"RoleRevoked\",\"type\":\"event\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"safeTransferFrom\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"},{\"internalType\":\"bytes\",\"name\":\"_data\",\"type\":\"bytes\"}],\"name\":\"safeTransferFrom\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"},{\"internalType\":\"bool\",\"name\":\"approved\",\"type\":\"bool\"}],\"name\":\"setApprovalForAll\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":true,\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"indexed\":true,\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"Transfer\",\"type\":\"event\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"to\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"transferFrom\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"unpause\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"Unpaused\",\"type\":\"event\"},{\"inputs\":[{\"components\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"itemType\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"strength\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"level\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"expireTime\",\"type\":\"uint256\"},{\"internalType\":\"bytes\",\"name\":\"signature\",\"type\":\"bytes\"}],\"internalType\":\"struct GameItem.ItemInfo\",\"name\":\"itemInfo\",\"type\":\"tuple\"}],\"name\":\"updateTokenWithSignedMessage\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"owner\",\"type\":\"address\"}],\"name\":\"balanceOf\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"DEFAULT_ADMIN_ROLE\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"getApproved\",\"outputs\":[{\"internalType\":\"address\",\"name\":\"\",\"type\":\"address\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"}],\"name\":\"getRoleAdmin\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"getTokenDetails\",\"outputs\":[{\"components\":[{\"internalType\":\"uint256\",\"name\":\"itemType\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"strength\",\"type\":\"uint256\"},{\"internalType\":\"uint256\",\"name\":\"level\",\"type\":\"uint256\"}],\"internalType\":\"struct GameItem.Item\",\"name\":\"\",\"type\":\"tuple\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes32\",\"name\":\"role\",\"type\":\"bytes32\"},{\"internalType\":\"address\",\"name\":\"account\",\"type\":\"address\"}],\"name\":\"hasRole\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"owner\",\"type\":\"address\"},{\"internalType\":\"address\",\"name\":\"operator\",\"type\":\"address\"}],\"name\":\"isApprovedForAll\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"MINTER_ROLE\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"name\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"ownerOf\",\"outputs\":[{\"internalType\":\"address\",\"name\":\"\",\"type\":\"address\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"paused\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"PAUSER_ROLE\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"bytes4\",\"name\":\"interfaceId\",\"type\":\"bytes4\"}],\"name\":\"supportsInterface\",\"outputs\":[{\"internalType\":\"bool\",\"name\":\"\",\"type\":\"bool\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"symbol\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"index\",\"type\":\"uint256\"}],\"name\":\"tokenByIndex\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"owner\",\"type\":\"address\"},{\"internalType\":\"uint256\",\"name\":\"index\",\"type\":\"uint256\"}],\"name\":\"tokenOfOwnerByIndex\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"uint256\",\"name\":\"tokenId\",\"type\":\"uint256\"}],\"name\":\"tokenURI\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"totalSupply\",\"outputs\":[{\"internalType\":\"uint256\",\"name\":\"\",\"type\":\"uint256\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"UPDATER_ROLE\",\"outputs\":[{\"internalType\":\"bytes32\",\"name\":\"\",\"type\":\"bytes32\"}],\"stateMutability\":\"view\",\"type\":\"function\"}]";
}

void UUpdateNFTExample::Init(FString _deviceId, FString _baseUrl, FString _session)
{
	deviceId = _deviceId;
	baseUrl = _baseUrl;
	session = _session;
}

void UUpdateNFTExample::GetNFTInfo(FString abi_hash, int tokenId, FMirageDelegate Result)
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

void UUpdateNFTExample::UpdateNFT(FString abi_hash, FItemInfoStructure _item, FMirageDelegate Result)
{
	http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda([Result, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - UpdateNFT - GetContentAsString: %s"), *Response->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString ticket = JsonObject->GetStringField("ticket");
				UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - UpdateNFT - ticket: %s"), *ticket);
				Result.ExecuteIfBound(ticket);
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
			item.strength++;
			item.signature = "0x";
			requestBody.args.Add(item);
			UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - UpdateNFT - %s"), *FRequestBodyStruct::ToJson(requestBody));

			Request->SetContentAsString(FRequestBodyStruct::ToJson(requestBody));
			Request->ProcessRequest();

#if PLATFORM_ANDROID
			FPlatformProcess::LaunchURL(session.GetCharArray().GetData(), NULL, NULL);
#endif
		});
}

void UUpdateNFTExample::GetTicketResult(FString ticketId, FMirageTicketResult Result)
{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
		Request->OnProcessRequestComplete().BindLambda([Result, ticketId, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
			{
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
				UE_LOG(LogTemp, Warning, TEXT("UpdateNFTExample - GetTicketResult - GetContentAsString: %s"), *Response->GetContentAsString());
				if (FJsonSerializer::Deserialize(Reader, JsonObject))
				{
					FString data = JsonObject->GetStringField("data");
					Result.ExecuteIfBound("Transaction Hash: " + data, 1);
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