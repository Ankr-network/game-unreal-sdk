#pragma once

#include "GameFramework/SaveGame.h"
#include "MirageSaveGame.generated.h"

const FString DefaultSlotName = FString("MirageSDK");

UCLASS()
class MIRAGESDK_API UMirageSaveGame : public USaveGame
{
    GENERATED_BODY()

public:

    UPROPERTY(VisibleAnywhere, Category = "MirageSDK")
    FString UniqueId;

    static void Save(FString _uniqueId);
    static UMirageSaveGame* Load();
    static void Erase();
};
