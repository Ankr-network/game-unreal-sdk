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

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int tokenId;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int itemType;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int strength;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int level;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int expireTime;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString signature;

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
};

UCLASS()
class ANKRSDK_API UItemInfo : public UUserDefinedStruct
{
	GENERATED_BODY()
};
