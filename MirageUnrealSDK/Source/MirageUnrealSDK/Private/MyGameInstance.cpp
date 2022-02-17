// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::InitializeMirageSDK()
{
	UMirageClient* mirageClient = NewObject<UMirageClient>();
	mirageClient->HelloWorldFromPlugin();
}