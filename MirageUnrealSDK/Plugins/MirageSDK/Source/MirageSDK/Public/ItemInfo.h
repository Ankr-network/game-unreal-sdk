#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GenericPlatform/GenericPlatform.h"
#include <JsonObjectConverter.h>
#include "ItemInfo.generated.h"

USTRUCT(BlueprintType)
struct FItemInfoStructure
{
	GENERATED_BODY()

	UPROPERTY() uint64 tokenId;
	UPROPERTY() uint64 itemType;
	UPROPERTY() uint64 strength;
	UPROPERTY() uint64 level;
	UPROPERTY() uint64 expireTime;
	UPROPERTY() FString signature;

	static FString ToJson(FItemInfoStructure _item)
	{
		FString json;
		FJsonObjectConverter::UStructToJsonObjectString(_item, json);
		return json;
	}

	static FItemInfoStructure FromJson(FString json)
	{
		FItemInfoStructure object{};
		FJsonObjectConverter::JsonObjectStringToUStruct(json, &object, 0, 0);
		return object;
	}

	void Log()
	{
		UE_LOG(LogTemp, Warning, TEXT("tokenId: %d"), tokenId);
		UE_LOG(LogTemp, Warning, TEXT("itemType: %d"), itemType);
		UE_LOG(LogTemp, Warning, TEXT("strength: %d"), strength);
		UE_LOG(LogTemp, Warning, TEXT("level: %d"), level);
		UE_LOG(LogTemp, Warning, TEXT("expireTime: %d"), expireTime);
		UE_LOG(LogTemp, Warning, TEXT("signature: %s"), *signature);
	}
};

UCLASS()
class MIRAGESDK_API UItemInfo : public UUserDefinedStruct
{
	GENERATED_BODY()
};
