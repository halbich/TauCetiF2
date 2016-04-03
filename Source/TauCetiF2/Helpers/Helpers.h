

#pragma once

#include "Common/SaveGameCarrier.h"
#include "Helpers.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UHelpers : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static TArray<FString> GetAllSaveGameSlotNames();


	static void SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier);
	static bool SaveGameDataToFile(const FString& FullFilePath, USaveGameCarrier& ToSave);
	static bool LoadGameDataFromFile(const FString& FullFilePath, USaveGameCarrier& ToLoad);


	static FORCEINLINE 	FString UHelpers::GetSaveFilePath() {
		return FString::Printf(TEXT("%s\\SaveGames\\"), *FPaths::GameSavedDir());
	}

	static FORCEINLINE FString UHelpers::GetCleanSaveFileName(const FString& worldName, const FDateTime& saveTime) {
		auto name = worldName.Replace(TEXT(" "), TEXT("_"));
		name.ToLowerInline();
		auto date = saveTime.ToString(TEXT("%d_%m_%Y_%H_%M_%S"));

		return FString::Printf(TEXT("%s_%s"), *name, *date);
	}
};
