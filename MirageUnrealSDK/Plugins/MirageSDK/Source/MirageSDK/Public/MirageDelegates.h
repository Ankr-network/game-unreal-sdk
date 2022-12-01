#pragma once

#include "AdvertisementData.h"
#include "MirageDelegates.generated.h"

DECLARE_DYNAMIC_DELEGATE_FiveParams(FMirageCallCompleteDynamicDelegate, FString, response, FString, data, FString, optionalData, int, optionalCode, bool, optionalBool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FApplicationResume);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAdvertisementReceivedDelegate, FAdvertisementDataStructure, advertisementData);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAdvertisementVideoAdDownloadDelegate, FString, path);

UCLASS()
class MIRAGESDK_API UMirageDelegates : public UObject
{
	GENERATED_BODY()
};