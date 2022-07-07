#pragma once

#include "AdvertisementData.h"
#include "AnkrDelegates.generated.h"

DECLARE_DYNAMIC_DELEGATE_FiveParams(FAnkrCallCompleteDynamicDelegate, FString, response, FString, data, FString, optionalData, int, optionalCode, bool, optionalBool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FApplicationResume);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAdvertisementReceivedDelegate, FAdvertisementDataStructure, advertisementData);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAdvertisementVideoAdDownloadDelegate, FString, path);

UCLASS()
class ANKRSDK_API UAnkrDelegates : public UObject
{
	GENERATED_BODY()
};