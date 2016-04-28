#include "TauCetiF2.h"
#include "SaveGameCarrier.h"
#include "Helpers/Saving/ArchiveHelpers.h"
#include "Helpers/Helpers.h"

const uint8 USaveGameCarrier::CURRENT_VERSION = 10;

USaveGameCarrier::USaveGameCarrier() {
	SaveFileVersion = CURRENT_VERSION;
	ContainsFullSaveData = false;
	SaveLoaded = false;

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

			if (carrier->IsQuickSave && hasQuickSave)
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
		result->SaveName = TEXT("Rychlé uložení");
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

void USaveGameCarrier::GetSaveForNewGame()
{
	IsSystemSave = true;
	PlayerPosition = FVector(-600, 0, 90);
	PartOfDay = 0.5f;
	PlayerUseFPSCamera = true;


	BuildableBlocks.Empty();
	addSystemBuildableActions();

	BuildableBlocks.Add(makeBuildable(EBlockName::BaseCube, FVector(1, 1, 1)));

	BuildableBlocks.Add(makeBuildable(EBlockName::BaseCube, FVector(1, 1, 4)));
	BuildableBlocks.Add(makeBuildable(EBlockName::WindowCube, FVector(4, 4, 1)));
	//BuildableBlocks.Add(makeBuildable(EBlockName::Terminal, FVector(1, 1, 1)));
	BuildableBlocks.Add(makeBuildable(EBlockName::ConstructCube, FVector(1, 1, 1)));
	BuildableBlocks.Add(makeBuildable(EBlockName::ConstructCubeSide, FVector(2, 2, 2)));
	BuildableBlocks.Add(makeBuildable(EBlockName::ConstructCubeBody, FVector(5, 5, 5)));

	auto build1 = makeBuildable(EBlockName::Door, FVector(1, 1, 1));
	build1->AdditionalFlags.Add(TEXT("doorOpening"), (int32)EDoorOpening::Left);
	build1->DefaultBuildingRotation = FRotator(0, 180, 0);
	//BuildableBlocks.Add(build1);

	auto build2 = makeBuildable(EBlockName::Door, FVector(1, 1, 1));
	build2->AdditionalFlags.Add(TEXT("doorOpening"), (int32)EDoorOpening::Right);
	build2->DefaultBuildingRotation = FRotator(0, 180, 0);
	//BuildableBlocks.Add(build2);

	auto door1 = make(EBlockName::Door, FVector(-20, -20, 5), FVector(1, 1, 1), FRotator(0, 0, 0));
	door1->AdditionalFlags.Add(TEXT("doorOpening"), (int32)EDoorOpening::Left);
	//UsedBlocks.Add(door1);

	auto door2 = make(EBlockName::Door, FVector(-20, 30, 5), FVector(1, 1, 1), FRotator(0, 180, 0));
	door2->AdditionalFlags.Add(TEXT("doorOpening"), (int32)EDoorOpening::Right);

	//UsedBlocks.Add(door2);

	//UsedBlocks.Add(make(EBlockName::Terminal, FVector(-20, -30, 5), FVector(1, 1, 1), FRotator(0, 0, 0)));
	
	UsedBlocks.Add(make(EBlockName::ConstructCubeSide, FVector(0, 0, 5), FVector(1, 4, 2), FRotator(0, 0, 0)));

	return;

	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-11, 3, 0), FVector(4, 4, 1), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-10, 6, 0), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-10, 1, 0), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 4, 0), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-8, 4, 0), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-8, 1, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 1, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 6, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 1, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-8, 1, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-10, 1, 2), FVector(4, 4, 1), FRotator(180, 0, 90)));
	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-13, 4, 2), FVector(4, 4, 1), FRotator(180, 90, 90)));
	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-11, 6, 2), FVector(4, 4, 1), FRotator(180, 180, 90)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-10, 6, 5), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 4, 5), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-8, 6, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-11, 3, 5), FVector(4, 4, 1), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-8, 3, 5), FVector(1, 1, 4), FRotator(0, 180, 270)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-10, 1, 5), FVector(1, 1, 4), FRotator(0, 270, 270)));
	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-8, 3, 3), FVector(4, 4, 1), FRotator(0, 90, 90)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-8, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));


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