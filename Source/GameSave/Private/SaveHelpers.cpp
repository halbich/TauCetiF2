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


FInventoryTags USaveHelpers::MakeFromTags(UInventoryTags* inventoryTagsObject)
{
	FInventoryTags inventory;

	if (!inventoryTagsObject || !inventoryTagsObject->IsValidLowLevel())
		return inventory;

	inventory.CurrentActiveIndex = inventoryTagsObject->CurrentActiveIndex;

	for (auto invGroup : inventoryTagsObject->InventoryGroupList)
	{
		if (!invGroup || !invGroup->IsValidLowLevel())
			continue;

		FInventoryTagGroup tagGroup;

		tagGroup.Name = invGroup->Name;
		tagGroup.IsEnabled = invGroup->IsEnabled;

		for (auto group : invGroup->GroupList)
		{
			if (!group || !group->IsValidLowLevel())
				continue;

			FTagGroup grp;
			grp.GroupName = group->GroupName;

			grp.Tags = TArray<FString>(group->Tags);

			tagGroup.GroupList.Add(grp);
		}


		inventory.InventoryGroupList.Add(tagGroup);
	}
	return inventory;
}
