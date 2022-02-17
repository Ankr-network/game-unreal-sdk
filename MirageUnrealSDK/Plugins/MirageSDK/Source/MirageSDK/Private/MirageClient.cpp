#include "MirageClient.h"

UMirageClient::UMirageClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("MirageSDK initialized."));
}

void UMirageClient::HelloWorldFromPlugin()
{
	UE_LOG(LogTemp, Warning, TEXT("UMirageClient::HelloWorldFromPlugin."));
}