

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



bool UHelpers::ChangeLocalization(FString target)
{
	return FInternationalization::Get().SetCurrentCulture(target);
}

#pragma optimize("", off)
void UHelpers::GetCurrentLocalizations(UPARAM(ref)TArray<FString>& DisplayNames, UPARAM(ref)TArray<FString>& IsoNames)
{
	TArray<FCultureRef> LocalizedCulturesForGame;

	FInternationalization::Get().GetCulturesWithAvailableLocalization(FPaths::GetGameLocalizationPaths(), LocalizedCulturesForGame, false);

	for (auto c : LocalizedCulturesForGame)
	{
		DisplayNames.Add(c->GetNativeName());
		IsoNames.Add(c->GetTwoLetterISOLanguageName());
	}
	
}

FString UHelpers::GetCurrentCultureIsoName()
{
	return  FInternationalization::Get().GetCurrentCulture()->GetTwoLetterISOLanguageName();
}

#pragma optimize("", on)

void UHelpers::FatalError(const FName text)
{
	UE_LOG(LogTemp, Fatal, TEXT("%s"), *text.ToString());
}


