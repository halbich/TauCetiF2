

#include "TauCetiF2.h"
#include "TCF2SaveGame.h"
#include "Helpers/Helpers.h"


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

UTCF2SaveGame* UTCF2SaveGame::GetEmptySave()
{
	return  Cast<UTCF2SaveGame>(UGameplayStatics::CreateSaveGameObject(UTCF2SaveGame::StaticClass()));
}

USaveGameCarrier* UTCF2SaveGame::GetEmptyCarrier()
{
	return  NewObject<USaveGameCarrier>();
}


bool UTCF2SaveGame::SaveGameDataToFile(const FString& FullFilePath, USaveGameCarrier& ToSave)
{
	//note that the supplied FString must be the entire Filepath
	// 	if writing it out yourself in C++ make sure to use the \\
			// 	for example:

// 	FString SavePath = "C:\\MyProject\\MySaveDir\\mysavefile.save";
	FBufferArchive ToBinary;
	//Step 1: Variable Data -> Binary
	SaveLoadData(ToBinary, ToSave, true);
	//following along from above examples

	//presumed to be global var data, 
	//could pass in the data too if you preferred

	//No Data
	if (ToBinary.Num() <= 0) return false;
	//~

	//Step 2: Binary to Hard Disk
	if (FFileHelper::SaveArrayToFile(ToBinary, *FullFilePath))
	{
		// Free Binary Array 	
		ToBinary.FlushCache();
		ToBinary.Empty();

		print("Save Success!");
		return true;
	}

	// Free Binary Array 	
	ToBinary.FlushCache();
	ToBinary.Empty();

	print("File Could Not Be Saved!");

	return false;
}

bool UTCF2SaveGame::SaveBinary(USaveGameCarrier* carrier)
{
	if (!carrier || !carrier->IsValidLowLevel())
		return false;

	carrier->SavedDate = FDateTime::Now();

	print(carrier->WorldName);
	print(carrier->SaveName);

	auto saveName = UHelpers::GetCleanSaveFileName(carrier->WorldName, carrier->SavedDate);

	auto filePath = FString::Printf(TEXT("%s\\SaveGames\\%s.sav"), *FPaths::GameSavedDir(), *saveName);
	return SaveGameDataToFile(filePath, *carrier);
}

void UTCF2SaveGame::SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier, bool bFullObject = true)
{
	Ar << carrier.SaveName;
	Ar << carrier.WorldName;
	Ar << carrier.SavedDate;
	Ar << carrier.PlayedTime;

	carrier.SaveLoaded = true;
	carrier.ContainsFullSaveData = bFullObject;

	if (!bFullObject)
		return;

	Ar << carrier.TimeOfDay;

}