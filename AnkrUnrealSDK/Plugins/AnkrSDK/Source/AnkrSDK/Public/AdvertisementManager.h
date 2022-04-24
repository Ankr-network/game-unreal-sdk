#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnkrDelegates.h"
#include "AdvertisementManager.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ANKRSDK_API UAdvertisementManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	FHttpModule* http;
	FString deviceId;
	FString session;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString appId;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString language;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString activeAccount;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) int chainId;

	void Init(FString _deviceId, FString _session);
	void SetAccount(FString _account, int _chainId);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void InitializeAdvertisement(FString _appId, FString _language);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetAdvertisement(EAdvertisementType advertisementType, FAdvertisementReceivedDelegate advertisementData);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void StartSession();
};