#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MirageDelegates.h"
#include "UpdateNFTExample.generated.h"

UCLASS(Blueprintable, BlueprintType)
class MIRAGESDK_API UUpdateNFTExample : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	FHttpModule* http;
	FString baseUrl;
	FString deviceId;
	FString session;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString ContractAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString ABI;

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void Init(FString _deviceId, FString _baseUrl, FString _session);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetNFTInfo(FString abi_hash, int tokenId, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void UpdateNFT(FString abi_hash, FItemInfoStructure _item, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetTicketResult(FString ticketId, FMirageTicketResult Result);
};