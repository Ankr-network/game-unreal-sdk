#pragma once

#include "MirageDelegates.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FMirageDelegate, FString, StringOut);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMirageConnectionStatus, bool, status);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMirageTicket, FString, TicketId);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FMirageTicketResult, FString, Status, int, Code);

UCLASS()
class MIRAGESDK_API UMirageDelegates : public UObject
{
	GENERATED_BODY()
};