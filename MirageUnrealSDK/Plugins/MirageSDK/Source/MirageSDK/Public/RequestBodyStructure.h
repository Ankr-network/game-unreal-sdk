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

	void Log()
	{
		UE_LOG(LogTemp, Warning, TEXT("device_id: %s"), *device_id);
		UE_LOG(LogTemp, Warning, TEXT("contract_address: %s"), *contract_address);
		UE_LOG(LogTemp, Warning, TEXT("abi_hash: %s"), *abi_hash);
		UE_LOG(LogTemp, Warning, TEXT("method: %s"), *method);
		UE_LOG(LogTemp, Warning, TEXT("args: %d"), args.Num());
	}
};

UCLASS()
class MIRAGESDK_API URequestBodyStructure : public UUserDefinedStruct
{
	GENERATED_BODY()
};
