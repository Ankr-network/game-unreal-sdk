#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Core/Public/Math/Color.h"
#include "QRCodeGenerator.generated.h"

UCLASS()
class MIRAGESDK_API UQRCodeGenerator : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "QRCode")
	static UTexture2D* GenerateQRCode(FString code);

	UFUNCTION(BlueprintCallable, Category = "QRCode")
	static UTexture2D* GenerateQRCodeWithParams(FString code, int QRCodeVersion, int PixelNum, FColor Backcolor, FColor QRPixelColor);
};
