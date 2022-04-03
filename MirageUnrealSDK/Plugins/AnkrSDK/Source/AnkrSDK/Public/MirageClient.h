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
class ANKRSDK_API UMirageClient : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	FHttpModule* http;
	FString baseUrl;
	FString deviceId;
	FString session;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) TArray<FString> accounts;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString activeAccount;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) int chainId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) UUpdateNFTExample* updateNFTExample;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) UWearableNFTExample* wearableNFTExample;

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void Ping(FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	bool GetClient(FMirageConnectionStatus Callback);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetWalletInfo(FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	FString GetActiveAccount();

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetData(FString contract, FString abi, FString method, FString args, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SendTransaction(FString contract, FString abi_hash, FString method, FString args, FMirageTicket Ticket);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetTicketResult(FString ticketId, FMirageTicketResult Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SendABI(FString abi, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SignMessage(FString message, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetSignature(FString ticket, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
    void VerifyMessage(FString message, FString signature, FMirageDelegate Result);
};