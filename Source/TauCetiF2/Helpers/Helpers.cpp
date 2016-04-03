

#include "TauCetiF2.h"
#include "PlatformFeatures.h"
#include "GameFramework/SaveGame.h"
#include "FFileVisitor.h"
#include "Helpers.h"





TArray<FString> UHelpers::GetAllSaveGameSlotNames()
{
	SET_WARN_COLOR(COLOR_CYAN);
	USaveGameCarrier* t = NewObject<USaveGameCarrier>();
	auto files = GetSaveFilePath() + TEXT("customSave.sav");
	UE_LOG(LogTemp, Log, TEXT("SAve path %s"), *files);
	auto c = SaveGameDataToFile(files, *t);
	UE_LOG(LogTemp, Log, TEXT("result %s"), c);



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



void UHelpers::SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier)
{
	/*Ar << SaveDataInt32;
	Ar << SaveDataVector;
	Ar << SaveDataRotatorArray;*/
}


bool UHelpers::SaveGameDataToFile(const FString& FullFilePath, USaveGameCarrier& ToSave)
{
	//note that the supplied FString must be the entire Filepath
	// 	if writing it out yourself in C++ make sure to use the \\
		// 	for example:

// 	FString SavePath = "C:\\MyProject\\MySaveDir\\mysavefile.save";
	FBufferArchive ToBinary;
	//Step 1: Variable Data -> Binary
	SaveLoadData(ToBinary, ToSave);
	//following along from above examples

		//presumed to be global var data, 
		//could pass in the data too if you preferred

		//No Data
	if (ToBinary.Num() <= 0) return false;
	//~

	//Step 2: Binary to Hard Disk
	if (FFileHelper::SaveArrayToFile(ToBinary, *FullFilePath))
	{
		// Free Binary Array 	
		ToBinary.FlushCache();
		ToBinary.Empty();

		print("Save Success!");
		return true;
	}

	// Free Binary Array 	
	ToBinary.FlushCache();
	ToBinary.Empty();

	print("File Could Not Be Saved!");

	return false;
}


//I am using the sample save data from above examples as the data being loaded
bool UHelpers::LoadGameDataFromFile(const FString& FullFilePath, USaveGameCarrier& ToLoad) {
	//Load the data array,
	// 	you do not need to pre-initialize this array,
	//		UE4 C++ is awesome and fills it 
	//		with whatever contents of file are, 
	//		and however many bytes that is
	TArray<uint8> TheBinaryArray;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *FullFilePath))
	{
		print("FFILEHELPER:>> Invalid File");
		return false;
		//~~
	}

	//Testing
	print("Loaded File Size");
	print(FString::FromInt(TheBinaryArray.Num()));

	//File Load Error
	if (TheBinaryArray.Num() <= 0) return false;

	//~
	//		  Read the Data Retrieved by GFileManager
	//~

	FMemoryReader FromBinary = FMemoryReader(TheBinaryArray, true); //true, free data after done
	FromBinary.Seek(0);
	SaveLoadData(FromBinary, ToLoad);

	//~
	//								Clean up 
	//~
	FromBinary.FlushCache();

	// Empty & Close Buffer 
	TheBinaryArray.Empty();
	FromBinary.Close();

	return true;
}