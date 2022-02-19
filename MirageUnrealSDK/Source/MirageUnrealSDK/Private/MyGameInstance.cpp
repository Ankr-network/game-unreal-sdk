// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMirageClient* UMyGameInstance::GetMirageClient()
{
	if (mirageClient == nullptr)
	{
		mirageClient = NewObject<UMirageClient>();
	}

	return mirageClient;
}