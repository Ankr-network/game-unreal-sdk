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
	full_image,
	video,
	rewarded
};

UENUM(BlueprintType)
enum class EAdvertisementTextureType : uint8
{
	E_False,
	E_Video,
	E_Image
};

USTRUCT(BlueprintType)
struct FRedirectResponse
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString android;
	UPROPERTY(Transient = ios) FString ios;

	void Log()
	{
		UE_LOG(LogTemp, Warning, TEXT("FMetaResponse - android: %s"), *android);
		UE_LOG(LogTemp, Warning, TEXT("FMetaResponse - ios: %s"), *ios);
	}

	static FString ToJson(FRedirectResponse _item)
	{
		FString json;
		FJsonObjectConverter::UStructToJsonObjectString(_item, json);
		return json;
	}

	static FRedirectResponse FromJson(FString json)
	{
		FRedirectResponse object{};
		FJsonObjectConverter::JsonObjectStringToUStruct(json, &object, 0, 0);
		return object;
	}
};

USTRUCT(BlueprintType)
struct FMetaResponse
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString title;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString ok_button;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString cancel_button;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString description;

	void Log()
	{
		UE_LOG(LogTemp, Warning, TEXT("FRedirectResponse - title: %s"), *title);
		UE_LOG(LogTemp, Warning, TEXT("FRedirectResponse - ok_button: %s"), *ok_button);
		UE_LOG(LogTemp, Warning, TEXT("FRedirectResponse - cancel_button: %s"), *cancel_button);
		UE_LOG(LogTemp, Warning, TEXT("FRedirectResponse - description: %s"), *description);
	}
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
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString texture_type;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int texture_width;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int texture_height;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString texture_orientation;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int required_time_in_seconds;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int time_in_seconds;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) bool is_nft_campaign;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FRedirectResponse redirect;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FMetaResponse meta;

	void Log()
	{
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - ad_type: %s"), *ad_type);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - uuid: %s"), *uuid);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - expire_at: %d"), expire_at);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - texture_url: %s"), *texture_url);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - texture_type: %s"), *texture_type);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - engagement_url: %s"), *engagement_url);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - texture_width: %d"), texture_width);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - texture_height: %d"), texture_height);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - texture_orientation: %s"), *texture_orientation);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - required_time_in_seconds: %d"), required_time_in_seconds);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - time_in_seconds: %d"), time_in_seconds);
		UE_LOG(LogTemp, Warning, TEXT("FAdvertisementDataStructure - is_nft_campaign: %d"), is_nft_campaign);
		redirect.Log();
		meta.Log();
	}
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
		result.Log();
		
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
class MIRAGESDK_API UAdvertisementData : public UUserDefinedStruct
{
	GENERATED_BODY()
};
