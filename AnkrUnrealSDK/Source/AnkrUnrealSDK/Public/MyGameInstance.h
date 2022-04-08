// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AnkrClient.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANKRUNREALSDK_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AnkrSDK")
	UAnkrClient* ankrClient;

	UFUNCTION(BlueprintCallable, Category = "AnkrSDK")
	UAnkrClient* GetAnkrClient();
	
};
