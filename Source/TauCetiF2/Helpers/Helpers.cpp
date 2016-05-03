

#include "TauCetiF2.h"
#include "PlatformFeatures.h"
#include "GameFramework/SaveGame.h"
#include "FFileVisitor.h"
#include "Helpers.h"





TArray<FString> UHelpers::GetAllSaveGameSlots()
{

	TArray<FString> ret;

	ISaveGameSystem* SaveSystem = IPlatformFeaturesModule::Get().GetSaveGameSystem();

	// If we have a save system and a valid name..
	if (SaveSystem)
	{
		// From SaveGameSystem.h in the Unreal source code base.
		FString saveGamePath = FString::Printf(TEXT("%s/SaveGames/"), *FPaths::GameSavedDir());

		UE_LOG(LogTemp, Log, TEXT("Search path %s"), *saveGamePath);

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


UImage* UHelpers::GetImageForBlock(UBuildableBlockInfo* blockInfo)
{
	if (!blockInfo)
		return nullptr;

	auto tex = GetTexture2DForBlock(blockInfo);
	if (!tex)
		return nullptr;

	UImage* res = NewObject<UImage>();

	if (!res)
		return nullptr;

	res->SetBrushFromTexture(tex);

	return res;
}

UTexture2D* UHelpers::GetTexture2DForBlock(UBuildableBlockInfo* blockInfo)
{
	if (!blockInfo)
		return nullptr;


	const FString baseFolder = TEXT("Texture2D'/Game/Textures/HUD/%s");
	//Texture2D'/Game/Textures/HUD/EmptyHand.EmptyHand'
	if (blockInfo->IsEmptyHand)
		return GetTexture2DByName(*FString::Printf(*baseFolder, TEXT("EmptyHand.EmptyHand'")));




	auto id = (EBlockName)blockInfo->ID;

	switch (id)
	{

	case EBlockName::ConstructCube: return GetTexture2DByName(*FString::Printf(*baseFolder, TEXT("BuildingBox.BuildingBox'")));
	case EBlockName::BaseCube: return GetTexture2DByName(*FString::Printf(*baseFolder, TEXT("BaseBox.BaseBox'")));
	case EBlockName::WindowCube: return GetTexture2DByName(*FString::Printf(*baseFolder, TEXT("BaseWindow.BaseWindow'")));
	case EBlockName::ConstructCubeSide: return GetTexture2DByName(*FString::Printf(*baseFolder, TEXT("BuildingBoxSide.BuildingBoxSide'")));
	case EBlockName::BaseRamp: return GetTexture2DByName(*FString::Printf(*baseFolder, TEXT("BaseBoxSide.BaseBoxSide'")));
	case EBlockName::ConstructCubeBody: return GetTexture2DByName(*FString::Printf(*baseFolder, TEXT("BuildingBoxBody.BuildingBoxBody'")));
	case EBlockName::Terminal: return GetTexture2DByName(*FString::Printf(*baseFolder, TEXT("Terminal.Terminal'")));
	case EBlockName::Door: {
		auto doorOpening = blockInfo->AdditionalFlags.Find(GetNameForTag(ENamedTag::DoorOpening));
		if (doorOpening)
		{
			auto value = (EDoorOpening)(*doorOpening);
			if(value == EDoorOpening::Left)
				return GetTexture2DByName(*FString::Printf(*baseFolder, TEXT("DoorLeft.DoorLeft'")));
			else if(value == EDoorOpening::Right)
				return GetTexture2DByName(*FString::Printf(*baseFolder, TEXT("DoorRight.DoorRight'")));
		}
		return nullptr;
		break;
	}
						   
	default:
		return nullptr;
	}

}


void UHelpers::ChangeLocalization(FString target)
{
	FInternationalization::Get().SetCurrentCulture(target);
}

void UHelpers::FatalError(const FName text)
{
	UE_LOG(LogTemp, Fatal, TEXT("%s"), *text.ToString());
}


