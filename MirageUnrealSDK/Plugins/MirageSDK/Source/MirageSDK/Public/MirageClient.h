#pragma once

#include "CoreMinimal.h"
#include "MirageClient.generated.h"

UCLASS()
class MIRAGESDK_API UMirageClient : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION()
	void HelloWorldFromPlugin();
};