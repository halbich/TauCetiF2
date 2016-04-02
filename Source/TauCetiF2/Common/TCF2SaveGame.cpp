

#include "TauCetiF2.h"
#include "TCF2SaveGame.h"


UTCF2SaveGame::UTCF2SaveGame()
{
	SaveSlotName = TEXT("EmptySaveSlot");
	UserIndex = 0;
}

void UTCF2SaveGame::SaveGame()
{
	PlayerName = TEXT("PlayerOne");
	UTCF2SaveGame* SaveGameInstance = Cast<UTCF2SaveGame>(UGameplayStatics::CreateSaveGameObject(UTCF2SaveGame::StaticClass()));
	SaveGameInstance->PlayerName = PlayerName;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}


UTCF2SaveGame* UTCF2SaveGame::LoadGame()
{
	UTCF2SaveGame* LoadGameInstance = Cast<UTCF2SaveGame>(UGameplayStatics::CreateSaveGameObject(UTCF2SaveGame::StaticClass()));
	LoadGameInstance = Cast<UTCF2SaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	FString PlayerNameToDisplay = LoadGameInstance->PlayerName;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, PlayerNameToDisplay);
	}

	return LoadGameInstance;
}

UTCF2SaveGame* UTCF2SaveGame::GetEmpty()
{
	return  Cast<UTCF2SaveGame>(UGameplayStatics::CreateSaveGameObject(UTCF2SaveGame::StaticClass()));
}