#include "TauCetiF2.h"
#include "SaveGameCarrier.h"
#include "Helpers/Helpers.h"

const uint8 USaveGameCarrier::CURRENT_VERSION = 1;

USaveGameCarrier::USaveGameCarrier() {
	SaveFileVersion = CURRENT_VERSION;
	ContainsFullSaveData = false;
	SaveLoaded = false;


	SaveName = TEXT("Pr�zdn� pozice");
	//PlayedTime(0);

	TimeOfDay = 0;
}

USaveGameCarrier* USaveGameCarrier::GetEmptyCarrier()
{
	return  NewObject<USaveGameCarrier>();
}


bool USaveGameCarrier::SaveGameDataToFile(const FString& FullFilePath)
{
	FBufferArchive ToBinary;
	SaveLoadData(ToBinary, *this);

	if (ToBinary.Num() <= 0)
		return false;

	if (FFileHelper::SaveArrayToFile(ToBinary, *FullFilePath))
	{
		ToBinary.FlushCache();
		ToBinary.Empty();

		return true;
	}

	ToBinary.FlushCache();
	ToBinary.Empty();

	return false;
}

bool USaveGameCarrier::SaveBinary()
{
	SavedDate = FDateTime::Now();

	if (FullFilePath.Len() == 0) {

		auto saveName = UHelpers::GetCleanSaveFileName(TEXT("tcf2"), SavedDate);
		FullFilePath = FString::Printf(TEXT("%s\\SaveGames\\%s.sav"), *FPaths::GameSavedDir(), *saveName);
	}

	return SaveGameDataToFile(FullFilePath);
}

bool USaveGameCarrier::LoadBinary(const FString& FilePath)
{
	return LoadGameDataFromFile(FilePath, true);
}

TArray<USaveGameCarrier*> USaveGameCarrier::GetSaveGameInfoList()
{
	TArray<USaveGameCarrier*> result;

	auto saves = UHelpers::GetAllSaveGameSlots();

	for (auto save : saves)
	{
		auto carrier = NewObject<USaveGameCarrier>();
		if (carrier->LoadGameDataFromFile(save, false)) {
			result.Add(carrier);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Found save with incorrect version(%d / current: %d). Deleting %s."), carrier->SaveFileVersion, CURRENT_VERSION, *save);
			FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*save);
		}
	}

	result.Sort([](const USaveGameCarrier& A, const USaveGameCarrier& B) {
		return A.SavedDate > B.SavedDate;
	});

	return result;
}

void USaveGameCarrier::GetSaveForNewGame()
{
}

bool USaveGameCarrier::LoadGameDataFromFile(const FString& FullFilePath, bool bFullObject) {

	TArray<uint8> TheBinaryArray;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *FullFilePath))
	{
		print("FFILEHELPER:>> Invalid File");
		return false;
	}

	//File Load Error
	if (TheBinaryArray.Num() <= 0)
		return false;

	//~
	//		  Read the Data Retrieved by GFileManager
	//~

	FMemoryReader FromBinary = FMemoryReader(TheBinaryArray, true); //true, free data after done
	FromBinary.Seek(0);
	SaveLoadData(FromBinary, *this, bFullObject);


	//~
	//								Clean up 
	//~
	FromBinary.FlushCache();

	// Empty & Close Buffer 
	TheBinaryArray.Empty();
	FromBinary.Close();

	this->FullFilePath = FString(FullFilePath);

	return SaveLoaded;
}

void USaveGameCarrier::SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier, bool bFullObject)
{
	Ar << carrier.SaveFileVersion;

	if (carrier.SaveFileVersion != CURRENT_VERSION)
		return;

	Ar << carrier.SaveName;
	Ar << carrier.SavedDate;
	Ar << carrier.PlayedTime;

	carrier.SaveLoaded = true;
	carrier.ContainsFullSaveData = bFullObject;

	if (!bFullObject)
		return;

	Ar << carrier.TimeOfDay;

}