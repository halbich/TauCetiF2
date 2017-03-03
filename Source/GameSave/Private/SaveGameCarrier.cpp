#include "GameSave.h"
#include "SaveGameCarrier.h"
#include "ArchiveHelpers.h"
#include "SaveHelpers.h"

const uint8 USaveGameCarrier::CURRENT_VERSION = 19;

USaveGameCarrier::USaveGameCarrier() {
	SaveFileVersion = CURRENT_VERSION;
	ContainsFullSaveData = false;
	SaveLoaded = false;

	HardcodedLevelName = ENamedHardcodedLevel::HardcodedLevelsMax;

	usedBlocks = TArray<FBlockInfo>();

	inventoryTags = FInventoryTags();

	buildableBlocks = TArray<FInventoryBuildableBlockInfo>();
}

USaveGameCarrier::~USaveGameCarrier() {
	usedBlocks.Empty();

	//inventoryTags = FInventoryTags();

	buildableBlocks.Empty();
}

USaveGameCarrier* USaveGameCarrier::GetEmptyCarrier()
{
	return  NewObject<USaveGameCarrier>();
}

USaveGameCarrier* USaveGameCarrier::GetQuickSaveCarrier()
{
	USaveGameCarrier* result = nullptr;

	auto saves = USaveHelpers::GetAllSaveGameSlots();
	bool hasQuickSave = false;
	for (auto save : saves)
	{
		auto carrier = NewObject<USaveGameCarrier>();
		if (carrier->LoadGameDataFromFile(save, false)) {
			if (!carrier->IsQuickSave)
				continue;

			if (hasQuickSave)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found another quick save. Deleting %s."), *save);
				FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*save);
			}
			else {
				hasQuickSave |= carrier->IsQuickSave;

				result = carrier;
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Found save with incorrect version(%d / current: %d). Deleting %s."), carrier->SaveFileVersion, CURRENT_VERSION, *save);
			FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*save);
		}
	}

	if (!result)
	{
		result = GetEmptyCarrier();
		// TODO Localization!
		result->SaveName = TEXT("Rychlé uložení");
		result->IsQuickSave = true;
	}

	return result;
}

bool USaveGameCarrier::SaveGameDataToFile(const FString& saveFilePath)
{
	FBufferArchive ToBinary;
	SaveLoadData(ToBinary, *this);

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

bool USaveGameCarrier::SaveBinary()
{
	SavedDate = FDateTime::Now();

	if (FullFilePath.Len() == 0) {
		auto saveName = USaveHelpers::GetCleanSaveFileName(IsQuickSave ? TEXT("tcf2_quick") : TEXT("tcf2"), SavedDate);
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

	auto saves = USaveHelpers::GetAllSaveGameSlots();
	bool hasQuickSave = false;
	for (auto save : saves)
	{
		auto carrier = NewObject<USaveGameCarrier>();
		if (carrier->LoadGameDataFromFile(save, false)) {
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

bool USaveGameCarrier::LoadGameDataFromFile(const FString& saveGameFile, bool bFullObject) {
	TArray<uint8> TheBinaryArray;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *saveGameFile))
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

	this->FullFilePath = FString(saveGameFile);

	return SaveLoaded;
}

bool USaveGameCarrier::IsSaveCompatible(const USaveGameCarrier& carrier)
{
	return carrier.SaveFileVersion == CURRENT_VERSION;
}

void USaveGameCarrier::SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier, bool bFullObject)
{
	Ar << carrier.SaveFileVersion;

	if (carrier.SaveFileVersion != CURRENT_VERSION)
		return;

	Ar << carrier.SaveName;
	Ar << carrier.SavedDate;
	Ar << carrier.PlayedTime;
	Ar << carrier.IsQuickSave;

	carrier.SaveLoaded = true;
	carrier.ContainsFullSaveData = bFullObject;

	if (!bFullObject)
		return;

	Ar << carrier.MinBoxSize;
	Ar << carrier.PartOfDay;

	Ar << carrier.PlayerPosition;
	Ar << carrier.PlayerRotation;
	Ar << carrier.PlayerCameraRotation;
	Ar << carrier.PlayerUseFPSCamera;

	Ar << carrier.usedBlocks;
	Ar << carrier.buildableBlocks;

	Ar << carrier.inventoryTags;
}

bool USaveGameCarrier::IsSaveNameValid()
{
	return SaveName.Len() > 0;
}