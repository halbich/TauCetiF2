#include "TauCetiF2.h"
#include "SaveGameCarrier.h"
#include "Helpers/Saving/ArchiveHelpers.h"
#include "Helpers/Helpers.h"

const uint8 USaveGameCarrier::CURRENT_VERSION = 15;

USaveGameCarrier::USaveGameCarrier() {
	SaveFileVersion = CURRENT_VERSION;
	ContainsFullSaveData = false;
	SaveLoaded = false;

	HardcodedLevelName = ENamedHardcodedLevel::HardcodedLevelsMax;
		
}





USaveGameCarrier* USaveGameCarrier::GetEmptyCarrier()
{
	return  NewObject<USaveGameCarrier>();
}

USaveGameCarrier* USaveGameCarrier::GetQuickSaveCarrier()
{
	USaveGameCarrier* result = nullptr;

	auto saves = UHelpers::GetAllSaveGameSlots();
	bool hasQuickSave = false;
	for (auto save : saves)
	{
		auto carrier = NewObject<USaveGameCarrier>();
		if (carrier->LoadGameDataFromFile(save, false)) {

			if (!carrier->IsQuickSave)
				continue;

			if ( hasQuickSave)
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
		result->SaveName = TEXT("Rychl� ulo�en�");
		result->IsQuickSave = true;
	}


	return result;
}


bool USaveGameCarrier::SaveGameDataToFile(const FString& FullFilePath)
{
	updateBeforeSave();
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

bool USaveGameCarrier::DeleteSaveFile()
{
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullFilePath))

	return FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FullFilePath);

	return false;
}

bool USaveGameCarrier::SaveBinary()
{
	SavedDate = FDateTime::Now();

	if (FullFilePath.Len() == 0) {

		auto saveName = UHelpers::GetCleanSaveFileName(IsQuickSave ? TEXT("tcf2_quick") : TEXT("tcf2"), SavedDate);
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

	updateAfterLoad();

	return SaveLoaded;
}

bool USaveGameCarrier::IsSaveCompatible(const USaveGameCarrier & carrier)
{
	return true;
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

}

bool USaveGameCarrier::IsSaveNameValid()
{
	return SaveName.Len() > 0;

}