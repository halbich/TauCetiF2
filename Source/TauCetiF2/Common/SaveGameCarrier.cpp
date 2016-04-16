#include "TauCetiF2.h"
#include "SaveGameCarrier.h"
#include "Helpers/Saving/ArchiveHelpers.h"
#include "Helpers/Helpers.h"

const uint8 USaveGameCarrier::CURRENT_VERSION = 8;

USaveGameCarrier::USaveGameCarrier() {
	SaveFileVersion = CURRENT_VERSION;
	ContainsFullSaveData = false;
	SaveLoaded = false;


	SaveName = TEXT("Prázdná pozice");
	//PlayedTime(0);


}





USaveGameCarrier* USaveGameCarrier::GetEmptyCarrier()
{
	return  NewObject<USaveGameCarrier>();
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


	BuildableBlocks.Add(NewObject<UBuildableBlockInfo>());
	BuildableBlocks[0]->IsEmptyHand = true;
	BuildableBlocks.Add(makeBuildable(EBlockName::BaseCube, FVector(4, 4, 4)));


	UsedBlocks.Add(make(EBlockName::Terminal, FVector(-30, -50, 5), FVector(1, 1, 1), FRotator(0, 0, 0)));

	//UsedBlocks.Add(make(EBlockName::BaseCube,FVector(0, 0, 1), FVector(4,4,4) , FRotator::ZeroRotator));

	//return;

	FRotator baseRotation(0, 0, 0);
	FVector BaseSize(20, 20, 4);



	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-10, -10, 1), BaseSize, FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(10, -10, 1), BaseSize, FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-10, 10, 1), BaseSize, FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::BaseCube, FVector(10, 10, 1), BaseSize, FRotator(0, 0, 0)));


	UsedBlocks.Add(make(EBlockName::BaseRamp, FVector(-26, 0, 1), FVector(8, 12, 4), FRotator(0, 90, 0)));


	FVector RoofSize(20, 20, 1);

	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-10, -10, 20), RoofSize, FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(10, -10, 20), RoofSize, FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-10, 10, 20), RoofSize, FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(10, 10, 20), RoofSize, FRotator(0, 0, 0)));

	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-9, -19, 5), FVector(19, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(10, -19, 5), FVector(19, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(0, -19, 13), FVector(8, 1, 12), FRotator(0, 0, 0)));


	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-9, +20, 5), FVector(19, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(10, +20, 5), FVector(19, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(0, 20, 13), FVector(8, 1, 12), FRotator(0, 0, 0)));

	// sloup
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(0, 0, 11), FVector(2, 2, 16), FRotator(0, 0, 0)));

	// vstup
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-19, -12, 11), FVector(1, 16, 16), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(20, -12, 11), FVector(1, 16, 16), FRotator(0, 0, 0)));


	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-19, 12, 11), FVector(1, 16, 16), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(20, 12, 11), FVector(1, 16, 16), FRotator(0, 0, 0)));


	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(20, 0, 11), FVector(1, 8, 16), FRotator(0, 0, 0)));


	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-11, -19, 13), FVector(15, 1, 12), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-11, 20, 13), FVector(15, 1, 12), FRotator(0, 0, 0)));

	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(12, -19, 13), FVector(15, 1, 12), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::WindowCube, FVector(12, 20, 13), FVector(15, 1, 12), FRotator(0, 0, 0)));

	UsedBlocks.Add(make(EBlockName::ConstructCubeBody, FVector(-30, -30, 13), FVector(10, 5, 2), FRotator(0, 0, 0)));
	UsedBlocks.Add(make(EBlockName::ConstructCubeSide, FVector(-30, -50, 13), FVector(10, 5, 2), FRotator(0, 0, 0)));



	/*FVector loc;
	FVector blockScale(1, 1, 1);

	UsedBlocks.Add(make(loc, blockScale + FVector(4, 4, 1), blockRotation, EShapeType::Cube, EBlockType::Base, EMaterialType::Steel));


	loc += FVector(2, 6, 0);
	loc += FVector(2, 0, 0);

	UsedBlocks.Add(make(loc, blockScale + FVector(1, 0, 1), blockRotation, EShapeType::CubeBody, EBlockType::Empty, EMaterialType::Polycarbon));

	loc = FVector(0, -5, 0);
	UsedBlocks.Add(make(loc, blockScale, blockRotation, EShapeType::Cube, EBlockType::Building, EMaterialType::Polycarbon));*/







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

	Ar << carrier.usedBlocks;
	Ar << carrier.buildableBlocks;

}

