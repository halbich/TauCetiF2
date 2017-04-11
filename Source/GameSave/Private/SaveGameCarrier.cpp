#include "GameSave.h"
#include "SaveGameCarrier.h"
#include "ArchiveHelpers.h"
#include "SaveHelpers.h"

const uint8 USaveGameCarrier::CURRENT_VERSION = 27;

USaveGameCarrier::USaveGameCarrier() {
	SaveFileVersion = CURRENT_VERSION;
	ContainsFullSaveData = false;
	SaveLoaded = false;

	HardcodedLevelName = ENamedHardcodedLevel::HardcodedLevelsMax;

	usedBlocks = TArray<FBlockInfo>();
	buildableBlocks = TArray<FInventoryBuildableBlockInfo>();
	inventoryBuildableBlocks = TArray<FInventoryBuildableItemBlockInfo>();

	inventoryTags = FInventoryTags();

}

USaveGameCarrier::~USaveGameCarrier() {
	usedBlocks.Empty();
	buildableBlocks.Empty();
	inventoryBuildableBlocks.Empty();

	inventoryTags.Empty();
}

USaveGameCarrier* USaveGameCarrier::GetEmptyCarrier()
{
	return  NewObject<USaveGameCarrier>();
}

USaveGameCarrier* USaveGameCarrier::GetQuickSaveCarrier(TArray<FText>& errorList)
{
	auto saves = USaveHelpers::GetAllSaveGameSlots();
	bool hasQuickSave = false;
	for (auto save : GetSaveGameInfoList(errorList))
	{
		if (save->IsQuickSave)
			return save;
	}

	auto result = GetEmptyCarrier();
	result->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveGameCarrier.QuickSave", "Rychlé uložení").ToString();
	result->IsQuickSave = true;

	return result;
}

bool USaveGameCarrier::SaveGameDataToFile(const FString& saveFilePath, TArray<FText>& errorList)
{
	FBufferArchive ToBinary;
	SaveLoadData(ToBinary, *this, errorList);

	if (ToBinary.Num() <= 0)
		return false;

	if (FFileHelper::SaveArrayToFile(ToBinary, *saveFilePath))
	{
		ToBinary.FlushCache();
		ToBinary.Empty();

		return true;
	}

	ToBinary.FlushCache();
	ToBinary.Empty();

	return false;
}

bool USaveGameCarrier::DeleteSaveFile()
{
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullFilePath))
		return FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FullFilePath);

	return false;
}

bool USaveGameCarrier::SaveBinary(TArray<FText>& errorList)
{
	SavedDate = FDateTime::Now();

	if (IsQuickSave || FullFilePath.Len() == 0) {
		auto saveName = USaveHelpers::GetCleanSaveFileName(IsQuickSave ? TEXT("tcf2_quick") : TEXT("tcf2"), SavedDate);
		FullFilePath = FString::Printf(TEXT("%s/SaveGames/%s.sav"), *FPaths::GameSavedDir(), *saveName);
		FPaths::NormalizeFilename(FullFilePath);

		if (IsQuickSave)
		{
			auto saves = USaveHelpers::GetAllSaveGameSlots();
			for (auto save : saves)
			{
				auto carrier = NewObject<USaveGameCarrier>();
				if (carrier->LoadGameDataFromFile(save, errorList, false) && carrier->IsQuickSave) {
					FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*save);			// we need to cleanup saves
				}
			}
		}
	}

	DEBUGPrintSave();

	return SaveGameDataToFile(FullFilePath, errorList);
}

bool USaveGameCarrier::LoadBinary(const FString& FilePath, TArray<FText>& errorList)
{
	return LoadGameDataFromFile(FilePath, errorList, true);
}

TArray<USaveGameCarrier*> USaveGameCarrier::GetSaveGameInfoList(TArray<FText>& errorList)
{
	TArray<USaveGameCarrier*> result;

	auto saves = USaveHelpers::GetAllSaveGameSlots();
	bool hasQuickSave = false;
	for (auto save : saves)
	{
		auto carrier = NewObject<USaveGameCarrier>();
		if (carrier->LoadGameDataFromFile(save, errorList, false)) {
			if (carrier->IsQuickSave && hasQuickSave)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found another quick save. Deleting %s."), *save);
				FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*save);
			}
			else {
				hasQuickSave |= carrier->IsQuickSave;
				result.Add(carrier);
			}
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

bool USaveGameCarrier::LoadGameDataFromFile(const FString& saveGameFile, TArray<FText>& errorList, bool bFullObject) {
	TArray<uint8> TheBinaryArray;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *saveGameFile))
	{
		errorList.Add(NSLOCTEXT("TCF2LocSpace", "LC.LoadSaveError.InvalidFile", "Soubor savu se nepodařilo otevřít. Soubor je pravděpodobně poškozen."));
		return false;
	}

	//File Load Error
	if (TheBinaryArray.Num() <= 0)
	{
		errorList.Add(NSLOCTEXT("TCF2LocSpace", "LC.LoadSaveError.InvalidFile_Empty", "Soubor savu není validní, neobsahuje žádná data."));

		return false;
	}

	//~
	//		  Read the Data Retrieved by GFileManager
	//~

	FMemoryReader FromBinary = FMemoryReader(TheBinaryArray, true); //true, free data after done
	FromBinary.Seek(0);
	SaveLoadData(FromBinary, *this, errorList, bFullObject);

	//~
	//								Clean up
	//~
	FromBinary.FlushCache();

	// Empty & Close Buffer
	TheBinaryArray.Empty();
	FromBinary.Close();

	this->FullFilePath = FString(saveGameFile);

	return SaveLoaded;
}

void USaveGameCarrier::SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier, TArray<FText>& errorList, bool bFullObject)
{
	Ar << carrier.SaveFileVersion;

	if (carrier.SaveFileVersion != CURRENT_VERSION)
	{
		errorList.Add(NSLOCTEXT("TCF2LocSpace", "LC.LoadSaveError.IncompatibleVersion", "Uvedený formát savu neodpovídá aktuální verzi."));
		return;
	}

	Ar << carrier.SaveName;
	Ar << carrier.SavedDate;
	Ar << carrier.PlayedTime;
	Ar << carrier.IsQuickSave;

	carrier.SaveLoaded = true;
	carrier.ContainsFullSaveData = bFullObject;

	if (!bFullObject)
		return;

	Ar << carrier.MinBoxSize;
	Ar << carrier.CurrentTime;

	Ar << carrier.PlayerPosition;
	Ar << carrier.PlayerRotation;
	Ar << carrier.PlayerCameraRotation;
	Ar << carrier.PlayerUseFPSCamera;

	Ar << carrier.usedBlocks;
	Ar << carrier.buildableBlocks;
	Ar << carrier.inventoryBuildableBlocks;

	Ar << carrier.inventoryTags;

	Ar << carrier.PlayerOxygenComponent;
	Ar << carrier.PlayerElectricityComponent;

	Ar << carrier.IsCreativeMode;

	Ar << carrier.weatherState;
}

bool USaveGameCarrier::IsSaveNameValid()
{
	return SaveName.Len() > 0;
}