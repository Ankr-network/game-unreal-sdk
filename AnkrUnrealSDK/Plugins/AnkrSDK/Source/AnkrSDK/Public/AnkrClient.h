#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpdateNFTExample.h"
#include "WearableNFTExample.h"
#include "AnkrDelegates.h"
#include "AdvertisementManager.h"
#include "RequestBodyStructure.h"
#include "AnkrClient.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ANKRSDK_API UAnkrClient : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	FHttpModule* http;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString deviceId;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString session;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString walletConnectDeeplink;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) bool needLogin;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) TArray<FString> accounts;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString activeAccount;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) int chainId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) UUpdateNFTExample* updateNFTExample;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) UWearableNFTExample* wearableNFTExample;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) UAdvertisementManager* advertisementManager;

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void Ping(const FAnkrCallCompleteDynamicDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void ConnectWallet(const FAnkrCallCompleteDynamicDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetWalletInfo(const FAnkrCallCompleteDynamicDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	FString GetActiveAccount();

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetData(FString contract, FString abi, FString method, FString args, const FAnkrCallCompleteDynamicDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SendTransaction(FString contract, FString abi_hash, FString method, FString args, const FAnkrCallCompleteDynamicDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetTicketResult(FString ticketId, const FAnkrCallCompleteDynamicDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SendABI(FString abi, const FAnkrCallCompleteDynamicDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SignMessage(FString message, const FAnkrCallCompleteDynamicDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetSignature(FString ticket, const FAnkrCallCompleteDynamicDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
    void VerifyMessage(FString message, FString signature, const FAnkrCallCompleteDynamicDelegate& Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	FString GetLastRequest();

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SetLastRequest(FString _lastRequest);
};