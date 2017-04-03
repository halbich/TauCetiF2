#pragma once

#include "Object.h"
#include "Commons/Public/Enums.h"
#include "Commons/Public/GameDefinitions.h"
#include "ArchiveHelpers.h"
#include "SaveHelpers.h"
#include "BlockSaveInfo.h"
#include "Inventory/FInventoryTags.h"
#include "Inventory/InventoryArchiveHelpers.h"
#include "Weather/FWeatherState.h"
#include "Weather/WeatherArchiveHelpers.h"
#include "SaveGameCarrier.generated.h"

/**
 * Carrier for saving / loading game
 */
UCLASS(BlueprintType, Transient)
class GAMESAVE_API USaveGameCarrier : public UObject
{
	GENERATED_BODY()

public:
	USaveGameCarrier();

	~USaveGameCarrier();

	static const uint8 CURRENT_VERSION;

	// This pragma has only visual effect (collapsing code in Visual Studio)
	// According to http://stackoverflow.com/questions/9000479/equivalent-of-region-for-c this definition of C# region equivalent is working only in Visual Studio
	// if you have any problems with compilation, please remove lines with #pragma region and #pragma endregion

#pragma region Properties

	UPROPERTY(BlueprintReadOnly, Transient, Category = SaveGameCarrier)
		bool IsSystemSave;

	UPROPERTY(Transient)
		ENamedHardcodedLevel HardcodedLevelName;

	UPROPERTY(BlueprintReadOnly, Transient, Category = SaveGameCarrier)
		FString FullFilePath;

	UPROPERTY(BlueprintReadOnly, Transient, Category = SaveGameCarrier)
		bool ContainsFullSaveData;

	UPROPERTY(BlueprintReadOnly, Transient, Category = SaveGameCarrier)
		bool SaveLoaded;

	UPROPERTY(BlueprintReadOnly, Transient, Category = SaveGameCarrier)
		uint8 SaveFileVersion;

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		FString SaveName;

	UPROPERTY(BlueprintReadOnly, Transient, Category = SaveGameCarrier)
		FDateTime SavedDate;

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		FTimespan PlayedTime;

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		bool IsQuickSave;

	// game - related stuff

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		float CurrentTime;	// interval [0,86400]

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		uint8 MinBoxSize;

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		FVector PlayerPosition;

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		FRotator PlayerRotation;

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		FRotator PlayerCameraRotation;

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		bool PlayerUseFPSCamera;

	UPROPERTY(BlueprintReadOnly, Transient, Category = SaveGameCarrier)
		bool IsCreativeMode;

	// Serializable item
	FElectricityComponentInfo PlayerElectricityComponent;

	// Serializable item
	FOxygenComponentInfo PlayerOxygenComponent;

	// Serializable array
	TArray<FBlockInfo> usedBlocks;

	//serializable Item
	FInventoryTags inventoryTags;

	// Serializable array
	TArray<FInventoryBuildableBlockInfo> buildableBlocks;

	//Serializable array
	TArray<FInventoryBuildableItemBlockInfo> inventoryBuildableBlocks;

	//serializable Item
	FWeatherState weatherState;

#pragma endregion

	// functions

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static USaveGameCarrier* GetEmptyCarrier();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static USaveGameCarrier* GetQuickSaveCarrier(TArray<FText>& errorList);

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool DeleteSaveFile();

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool SaveBinary(TArray<FText>& errorList);

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool LoadBinary(const FString& FilePath, TArray<FText>& errorList);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static TArray<USaveGameCarrier*> GetSaveGameInfoList(TArray<FText>& errorList);

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool IsSaveNameValid();

private:

	void SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier, TArray<FText>& errorList, bool bFullObject = true);
	bool SaveGameDataToFile(const FString& FullFilePath, TArray<FText>& errorList);
	bool LoadGameDataFromFile(const FString& FullFilePath, TArray<FText>& errorList, bool bFullObject = true);

	void DEBUGPrintSave() {
#if WITH_EDITOR
		UE_LOG(LogTemp, Log, TEXT("~~~~~~~~~~~~~~~~~~  Save ~~~~~~~~~~~~~~~~~~~~~~~~"));

		for (auto info : usedBlocks)
		{
			UE_LOG(LogTemp, Log, TEXT("UsedBlocks->Add(make(%d, FVector(%d, %d, %d), FVector(%d, %d, %d), FRotator(%d, %d, %d)));"),
				info.ID,
				FMath::RoundToInt(info.Location.X), FMath::RoundToInt(info.Location.Y), FMath::RoundToInt(info.Location.Z),
				FMath::RoundToInt(info.Scale.X), FMath::RoundToInt(info.Scale.Y), FMath::RoundToInt(info.Scale.Z),
				FMath::RoundToInt(info.Rotation.Pitch), FMath::RoundToInt(info.Rotation.Yaw), FMath::RoundToInt(info.Rotation.Roll));
		}

		UE_LOG(LogTemp, Log, TEXT("c->PlayerPosition = FVector(%d, %d, %d);"),
			FMath::RoundToInt(PlayerPosition.X), FMath::RoundToInt(PlayerPosition.Y), FMath::RoundToInt(PlayerPosition.Z)
		);

		UE_LOG(LogTemp, Log, TEXT("c->PlayerRotation = FRotator(%d, %d, %d);"),
			FMath::RoundToInt(PlayerRotation.Pitch), FMath::RoundToInt(PlayerRotation.Yaw), FMath::RoundToInt(PlayerRotation.Roll)
		);

		UE_LOG(LogTemp, Log, TEXT("~~~~~~~~~~~~~~~~~~  Save  ends ~~~~~~~~~~~~~~~~~~~~~~~~"));

#endif
	}



};
