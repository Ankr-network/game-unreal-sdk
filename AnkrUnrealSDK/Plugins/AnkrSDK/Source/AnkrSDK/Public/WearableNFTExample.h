#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnkrDelegates.h"
#include "WearableNFTExample.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ANKRSDK_API UWearableNFTExample : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	FHttpModule* http;
	FString deviceId;
	FString session;
	FString hat;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString activeAccount;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) int chainId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameItemContractAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameItemABI;
			 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameCharacterContractAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameCharacterABI;
			  
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString TransactionGasLimit;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString BlueHatAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString RedHatAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString WhiteHatAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString BlueShoesAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString RedShoesAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString WhiteShoesAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString BlueGlassesAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString RedGlassesAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString WhiteGlassesAddress;

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void Init(FString _deviceId, FString _session);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void SetAccount(FString _account, int _chainId);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void MintItems(FString abi_hash, FString to, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void MintCharacter(FString abi_hash, FString to, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GameItemSetApproval(FString abi_hash, FString callOperator, bool approved, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetCharacterBalance(FString abi_hash, FString address, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetCharacterTokenId(FString abi_hash, int tokenBalance, FString owner, FString index, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void ChangeHat(FString abi_hash, int characterId, bool hasHat, FString hatAddress, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetHat(FString abi_hash, int characterId, FAnkrDelegate Result);
	
	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetTicketResult(FString ticketId, FAnkrTicketResult Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	void GetItemsBalance(FString abi_hash, FString address, FAnkrDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "ANKR SDK")
	int GetItemValueFromBalances(FString data, int index);
};