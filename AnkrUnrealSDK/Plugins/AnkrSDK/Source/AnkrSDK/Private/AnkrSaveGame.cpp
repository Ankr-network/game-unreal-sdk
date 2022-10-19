#include "AnkrSaveGame.h"
#include "Kismet/GameplayStatics.h"

UAnkrSaveGame* UAnkrSaveGame::Load()
{
    if (UGameplayStatics::DoesSaveGameExist("AnkrSDK", 0))
    {
        UAnkrSaveGame* load = Cast<UAnkrSaveGame>(UGameplayStatics::CreateSaveGameObject(UAnkrSaveGame::StaticClass()));
        load = Cast<UAnkrSaveGame>(UGameplayStatics::LoadGameFromSlot("AnkrSDK", 0));

        return load;
    }

    return nullptr;
}
void UAnkrSaveGame::Save(FString _uniqueId)
{
    UAnkrSaveGame* save = Cast<UAnkrSaveGame>(UGameplayStatics::CreateSaveGameObject(UAnkrSaveGame::StaticClass()));
    save->UniqueId = _uniqueId;

    UGameplayStatics::SaveGameToSlot(save, "AnkrSDK", 0);
}
void UAnkrSaveGame::Erase()
{
	if (UGameplayStatics::DoesSaveGameExist("AnkrSDK", 0))
	{
		UGameplayStatics::DeleteGameInSlot("AnkrSDK", 0);
	}
}
