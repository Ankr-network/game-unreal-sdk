// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UAnkrClient* UMyGameInstance::GetAnkrClient()
{
	if (ankrClient == nullptr)
	{
		ankrClient = NewObject<UAnkrClient>();
	}

	return ankrClient;
}