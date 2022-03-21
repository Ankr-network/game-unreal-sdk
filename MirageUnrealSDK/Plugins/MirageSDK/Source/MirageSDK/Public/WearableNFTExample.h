#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MirageDelegates.h"
#include "WearableNFTExample.generated.h"

UCLASS(Blueprintable, BlueprintType)
class MIRAGESDK_API UWearableNFTExample : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	FHttpModule* http;
	FString baseUrl;
	FString deviceId;
	FString session;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameItemContractAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameItemABI;
			 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameCharacterContractAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString GameCharacterABI;
			  
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString TransactionGasLimit;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString BlueHatAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString RedHatAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString BlueShoesAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString WhiteShoesAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString RedGlassesAddress;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) FString WhiteGlassesAddress;

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void Init(FString _deviceId, FString _baseUrl, FString _session);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void MintItems(FString abi_hash, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void MintCharacter(FString abi_hash, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GameItemSetApproval(FString abi_hash, FString callOperator, bool approved, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetCharacterBalance(FString address, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetBalanceERC1155(FString account, FString id, FMirageDelegate Result);


	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetCharacterTokenId(int64 tokenBalance, FString owner, FString index, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetHasHatToken(int64 tokenBalance, FString tokenAddress, FString account, FString id, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void ChangeHat(int64 characterId, bool hasHat, FString hatAddress, FMirageDelegate Result);

	UFUNCTION(BlueprintCallable, Category = "MirageSDK")
	void GetHat(FString abi_hash, int64 characterId, FMirageDelegate Result);
	
};