#pragma once

#include "AdvertisementData.h"
#include "AnkrDelegates.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FAnkrDelegate, FString, StringOut);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAnkrConnectionStatus, bool, status);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAnkrTicket, FString, TicketId);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FAnkrTicketResult, FString, Status, int, Code);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FApplicationResume);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAdvertisementReceivedDelegate, FAdvertisementDataStructure, advertisementData);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAdvertisementVideoAdDownloadDelegate, FString, path);

UCLASS()
class ANKRSDK_API UAnkrDelegates : public UObject
{
	GENERATED_BODY()
};