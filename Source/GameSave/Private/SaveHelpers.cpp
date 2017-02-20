#include "GameSave.h"
#include "PlatformFeatures.h"
#include "GameFramework/SaveGame.h"
#include "FFileVisitor.h"
#include "SaveHelpers.h"

TArray<FString> USaveHelpers::GetAllSaveGameSlots()
{
	TArray<FString> ret;

	ISaveGameSystem* SaveSystem = IPlatformFeaturesModule::Get().GetSaveGameSystem();

	// If we have a save system and a valid name..
	if (SaveSystem)
	{
		// From SaveGameSystem.h in the Unreal source code base.
		FString saveGamePath = FString::Printf(TEXT("%s/SaveGames/"), *FPaths::GameSavedDir());

		// Array of fstrings to pass back
		TArray<FString> filePaths;

		// Reference to the FPlatformManager
		FPlatformFileManager* myFileManager = new FPlatformFileManager;

		// Reference to the FFileMatch class that has the visit override for the Visitor needed for iterate directory
		FFileVisitor FileMatch(filePaths, FString(".sav"), true, false);

		// Check if the directory supplied exists
		if (FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*saveGamePath))
		{
			// Directory exists so loop through the files
			myFileManager->Get().GetPlatformFile().IterateDirectory(*saveGamePath, FileMatch);
		}

		// Return the array
		return filePaths;
	}

	return ret;
}

//
//FInventoryTags& USaveHelpers::MakeFromTags(UInventoryTags* inventoryTagsObject)
//{
//	FInventoryTags inventory;
//	//for (auto usedBlock : blockObjectArray)
//	//{
//	//	/*if (usedBlock)
//	//	blockArray.Add(usedBlock->ToContainer());*/
//	//}
//	/*Ar << block.ID;
//	Ar << block.Scale;
//	Ar << block.Name;
//	Ar << block.AdditionalFlags;*/
//	return inventory;
//}
