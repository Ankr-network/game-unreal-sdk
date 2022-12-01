#include "MirageSaveGame.h"
#include "Kismet/GameplayStatics.h"

UMirageSaveGame* UMirageSaveGame::Load()
{
    if (UGameplayStatics::DoesSaveGameExist(DefaultSlotName, 0))
    {
        UMirageSaveGame* load = Cast<UMirageSaveGame>(UGameplayStatics::CreateSaveGameObject(UMirageSaveGame::StaticClass()));
        load = Cast<UMirageSaveGame>(UGameplayStatics::LoadGameFromSlot(DefaultSlotName, 0));

        return load;
    }

    return nullptr;
}
void UMirageSaveGame::Save(FString _uniqueId)
{
    UMirageSaveGame* save = Cast<UMirageSaveGame>(UGameplayStatics::CreateSaveGameObject(UMirageSaveGame::StaticClass()));
    save->UniqueId = _uniqueId;

    UGameplayStatics::SaveGameToSlot(save, DefaultSlotName, 0);
}
void UMirageSaveGame::Erase()
{
	if (UGameplayStatics::DoesSaveGameExist(DefaultSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(DefaultSlotName, 0);
	}
}
