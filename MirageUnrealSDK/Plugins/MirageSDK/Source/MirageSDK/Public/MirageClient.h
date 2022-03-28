#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpdateNFTExample.h"
#include "WearableNFTExample.h"
#include "MirageDelegates.h"
#include "MirageClient.generated.h"

UCLASS(Blueprintable, BlueprintType)
class MIRAGESDK_API UMirageClient : public UObject
{
	GENERATED_UCLASS_BODY()

	
public:

	FHttpModule* http;
	FString baseUrl;
	FString deviceId;
	FString session;
	FString clientId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) UUpdateNFTExample* updateNFTExample;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) UWearableNFTExample* wearableNFTExample;

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void Ping(FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	bool GetClient(FMirageConnectionStatus Callback);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetData(FString contract, FString abi, FString method, FString args, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void SendTransaction(FString contract, FString abi_hash, FString method, FString args, FMirageTicket Ticket);

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