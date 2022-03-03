#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MirageClient.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FMirageDelegate, FString, StringOut);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMirageConnectionStatus, bool, status);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMirageTicket, FString, TicketId);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FMirageTicketResult, FString, Status, int, Code);

UCLASS(Blueprintable, BlueprintType)
class MIRAGESDK_API UMirageClient : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	FHttpModule* http;
	FString clientId;
	FString baseUrl;
	FString deviceId;
	FString session;

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	bool GetClient(FMirageConnectionStatus Callback);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetData(FString contract, FString abi, FString method, FString args, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void SendTransaction(FString contract, FString abi, FString method, FString args, FMirageTicket Ticket);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetTicketResult(FString ticketId, FMirageTicketResult Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void SendABI(FString abi, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void SignMessage(FString message, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetSignature(FString ticket, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
    void VerifyMessage(FString message, FString signature, FMirageDelegate Result);
};