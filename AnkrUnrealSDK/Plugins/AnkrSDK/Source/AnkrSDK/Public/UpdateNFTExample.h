#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnkrDelegates.h"
#include "UpdateNFTExample.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ANKRSDK_API UUpdateNFTExample : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	FHttpModule* http;
	FString baseUrl;
	FString deviceId;
	FString session;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString activeAccount;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) int chainId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString ContractAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString ABI;

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void Init(FString _deviceId, FString _baseUrl, FString _session);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SetAccount(FString _account, int _chainId);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetNFTInfo(FString abi_hash, int tokenId, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void UpdateNFT(FString abi_hash, FItemInfoStructure _item, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetTicketResult(FString ticketId, FAnkrTicketResult Result);
};