

#pragma once

#include "Object.h"
#include "SaveGameCarrier.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API USaveGameCarrier : public UObject
{
	GENERATED_BODY()

public:
	USaveGameCarrier();

	static const uint8 CURRENT_VERSION;


	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		bool ContainsFullSaveData;

	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		bool SaveLoaded;


	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		uint8 SaveFileVersion;

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FString SaveName;

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FString WorldName;

	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		FDateTime SavedDate;

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FTimespan PlayedTime;



	// game - related stuff


	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		uint8 TimeOfDay;


	// functions

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static USaveGameCarrier* GetEmptyCarrier();


	void SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier, bool bFullObject = true);
	bool SaveGameDataToFile(const FString& FullFilePath);

	bool LoadGameDataFromFile(const FString& FullFilePath, bool bFullObject = true);

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool SaveBinary();


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static TArray<USaveGameCarrier*> GetSaveGameInfoList();
};


