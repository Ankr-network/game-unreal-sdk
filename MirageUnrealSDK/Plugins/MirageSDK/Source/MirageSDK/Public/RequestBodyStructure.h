#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GenericPlatform/GenericPlatform.h"
#include <JsonObjectConverter.h>
#include "ItemInfo.h"
#include "RequestBodyStructure.generated.h"

USTRUCT(BlueprintType)
struct FRequestBodyStruct
{
	GENERATED_BODY()

	UPROPERTY() FString device_id;
	UPROPERTY() FString contract_address;
	UPROPERTY() FString abi_hash;
	UPROPERTY() FString method;
	UPROPERTY() TArray<FItemInfoStructure> args;

	static FString ToJson(FRequestBodyStruct _item)
	{
		FString json;
		FJsonObjectConverter::UStructToJsonObjectString(_item, json);
		return json;
	}

	static FRequestBodyStruct FromJson(FString json)
	{
		FRequestBodyStruct object{};
		FJsonObjectConverter::JsonObjectStringToUStruct(json, &object, 0, 0);
		return object;
	}
};

/*USTRUCT(BlueprintType)
struct FRequestJson
{
	GENERATED_BODY()

	UPROPERTY() FString device_id;
	UPROPERTY() FString contract_address;
	UPROPERTY() FString abi_hash;
	UPROPERTY() FString method;
	UPROPERTY() TArray<FString> args;

	static FString SetContent(FString device_id, FString contract_address, FString abi_hash, FString method, TArray<FString> args)
	{
		FRequestJson content{};
		content.device_id = device_id;
		content.contract_address = contract_address;
		content.abi_hash = abi_hash;
		content.method = method;
		content.args = args;

		FString json;
		FJsonObjectConverter::UStructToJsonObjectString(content, json);
		return json;
	}
};*/

UCLASS()
class MIRAGESDK_API URequestBodyStructure : public UUserDefinedStruct
{
	GENERATED_BODY()
};
