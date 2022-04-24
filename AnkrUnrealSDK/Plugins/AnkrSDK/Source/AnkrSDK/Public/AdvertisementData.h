#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Engine/UserDefinedStruct.h"
#include "GenericPlatform/GenericPlatform.h"
#include <JsonObjectConverter.h>
#include "AdvertisementData.generated.h"

UENUM(BlueprintType)
enum class EAdvertisementType : uint8
{
	banner,
	fullscreen
};

USTRUCT(BlueprintType)
struct FAdvertisementResponse
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString ad_type;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString uuid;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int expire_at;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString texture_url;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString engagement_url;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int texture_width;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int texture_height;
};

USTRUCT(BlueprintType)
struct FAdvertisementDataStructure
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int code;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString error;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FAdvertisementResponse result;

	void Log()
	{
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - code: %d"), code);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - error: %s"), *error);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - ad_type: %s"), *result.ad_type);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - uuid: %s"), *result.uuid);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - expire_at: %d"), result.expire_at);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - texture_url: %s"), *result.texture_url);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - engagement_url: %s"), *result.engagement_url);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - texture_width: %d"), result.texture_width);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - texture_height: %d"), result.texture_height);
	}

	static FString ToJson(FAdvertisementDataStructure _item)
	{
		FString json;
		FJsonObjectConverter::UStructToJsonObjectString(_item, json);
		return json;
	}

	static FAdvertisementDataStructure FromJson(FString json)
	{
		FAdvertisementDataStructure object{};
		FJsonObjectConverter::JsonObjectStringToUStruct(json, &object, 0, 0);
		return object;
	}
};

UCLASS()
class ANKRSDK_API UAdvertisementData : public UUserDefinedStruct
{
	GENERATED_BODY()
};
