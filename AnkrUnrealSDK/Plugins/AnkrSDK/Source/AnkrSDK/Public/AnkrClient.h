#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpdateNFTExample.h"
#include "WearableNFTExample.h"
#include "AnkrDelegates.h"
#include "AdvertisementManager.h"
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
	void Ping(FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void ConnectWallet(FAnkrConnectionStatus Callback);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetWalletInfo(FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	FString GetActiveAccount();

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetData(FString contract, FString abi, FString method, FString args, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SendTransaction(FString contract, FString abi_hash, FString method, FString args, FAnkrTicket Ticket);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetTicketResult(FString ticketId, FAnkrTicketResult Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SendABI(FString abi, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SignMessage(FString message, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetSignature(FString ticket, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
    void VerifyMessage(FString message, FString signature, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	FString GetLastRequest();

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SetLastRequest(FString _lastRequest);
};