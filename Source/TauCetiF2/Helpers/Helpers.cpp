

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





//I am using the sample save data from above examples as the data being loaded

void UHelpers::DrawDebugBox(AActor* caller, FVector start, FVector end) {

	if (!caller)
		return;

	auto world = caller->GetWorld();

	if (!world)
		return;

	auto b = FVector(FMath::Min(start.X, end.X), FMath::Min(start.Y, end.Y), FMath::Min(start.Z, end.Z));
	auto e = FVector(FMath::Max(start.X, end.X), FMath::Max(start.Y, end.Y), FMath::Max(start.Z, end.Z));

	print(TEXT("--------"));
	print(b.ToString());

	print(e.ToString());

	TArray<FVector> points;
	points.AddDefaulted(8);

	points[0] = b;
	points[1] = points[0] + FVector(e.X, 0, 0);
	points[2] = points[0] + FVector(0, e.Y, 0);
	points[3] = points[0] + FVector(e.X, e.Y, 0);

	auto up = FVector(0, 0, e.Z);
	points[4] = points[0] + up;
	points[5] = points[1] + up;
	points[6] = points[2] + up;
	points[7] = points[3] + up;

	FColor color = FColor::Red;
	DrawDebugLine(world, points[0], points[1], color, false, 20, 5);
	DrawDebugLine(world, points[0], points[2], color, false, 20, 5);
	DrawDebugLine(world, points[0], points[3], color, false, 20, 5);
	DrawDebugLine(world, points[0], points[4], color, false, 20, 5);
	DrawDebugLine(world, points[0], points[5], color, false, 20, 5);
	DrawDebugLine(world, points[0], points[6], color, false, 20, 5);
	//													, false, 5, 20
	DrawDebugLine(world, points[1], points[2], color, false, 20, 5);
	DrawDebugLine(world, points[1], points[3], color, false, 20, 5);
	DrawDebugLine(world, points[1], points[4], color, false, 20, 5);
	DrawDebugLine(world, points[1], points[5], color, false, 20, 5);
	DrawDebugLine(world, points[1], points[7], color, false, 20, 5);
	//													, false, 5, 20
	DrawDebugLine(world, points[2], points[3], color, false, 20, 5);
	DrawDebugLine(world, points[2], points[4], color, false, 20, 5);
	DrawDebugLine(world, points[2], points[6], color, false, 20, 5);
	DrawDebugLine(world, points[2], points[7], color, false, 20, 5);
	//													, false, 5, 20
	DrawDebugLine(world, points[3], points[3], color, false, 20, 5);
	DrawDebugLine(world, points[3], points[6], color, false, 20, 5);
	DrawDebugLine(world, points[3], points[7], color, false, 20, 5);
	//													, false, 5, 20
	DrawDebugLine(world, points[4], points[5], color, false, 20, 5);
	DrawDebugLine(world, points[4], points[6], color, false, 20, 5);
	DrawDebugLine(world, points[4], points[7], color, false, 20, 5);
	//													, false, 5, 20
	DrawDebugLine(world, points[5], points[6], color, false, 20, 5);
	DrawDebugLine(world, points[5], points[7], color, false, 20, 5);
	//													, false, 5, 20
	DrawDebugLine(world, points[6], points[7], color, false, 20, 5);
}