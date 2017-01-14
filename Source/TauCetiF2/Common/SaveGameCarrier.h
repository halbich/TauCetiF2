

#pragma once

#include "TauCetiF2.h"
#include "Game/GameDefinitions.h"
#include "Object.h"
#include "Commons/Public/Enums.h"
#include "Helpers/Saving/ArchiveHelpers.h"
#include "Helpers/Saving/BlockSaveInfo.h"
#include "Blocks/Info/BlockInfo.h"
#include "Blocks/Info/BuildableBlockInfo.h"
#include "Game/Inventory/InventoryTags.h"
#include "SaveGameCarrier.generated.h"

/**
 * Carrier for saving / loading game
 */
UCLASS(BlueprintType, Transient)
class TAUCETIF2_API USaveGameCarrier : public UObject
{
	GENERATED_BODY()

public:
	USaveGameCarrier();

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
		FText SystemSaveName;

	UPROPERTY(BlueprintReadOnly, Transient, Category = SaveGameCarrier)
		FDateTime SavedDate;

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		FTimespan PlayedTime;

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		bool IsQuickSave;

	// game - related stuff



	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		float PartOfDay;	// interval [0,1]

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


	// blocks 
	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		TArray<UBlockInfo*> UsedBlocks;

	// Serializable array
	TArray<FBlockInfo> usedBlocks;

	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		UInventoryTags* InventoryTags;

	//serializable Item
	FInventoryTags inventoryTags;

	// blocks 
	UPROPERTY(BlueprintReadWrite, Transient, Category = SaveGameCarrier)
		TArray<UBuildableBlockInfo*> BuildableBlocks;

	// Serializable array
	TArray<FInventoryBuildableBlockInfo> buildableBlocks;

#pragma endregion

	// functions

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static USaveGameCarrier* GetEmptyCarrier();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static USaveGameCarrier* GetQuickSaveCarrier();



	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool DeleteSaveFile();


	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool SaveBinary();

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool LoadBinary(const FString& FilePath);


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static TArray<USaveGameCarrier*> GetSaveGameInfoList();

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool IsSaveNameValid();


private:

	void SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier, bool bFullObject = true);
	bool SaveGameDataToFile(const FString& FullFilePath);
	bool LoadGameDataFromFile(const FString& FullFilePath, bool bFullObject = true);

	bool IsSaveCompatible(const USaveGameCarrier& carrier);

	void DEBUGPrintSave() {

#if WITH_EDITOR
		UE_LOG(LogTemp, Log, TEXT("~~~~~~~~~~~~~~~~~~  Save ~~~~~~~~~~~~~~~~~~~~~~~~"));

		for (auto info : usedBlocks)
		{

			UE_LOG(LogTemp, Log, TEXT("UsedBlocks.Add(make(EBlockName::%s, FVector(%d, %d, %d), FVector(%d, %d, %d), FRotator(%d, %d, %d)));"),
				TEXT("TODO dodelej me"),
				FMath::RoundToInt(info.Location.X), FMath::RoundToInt(info.Location.Y), FMath::RoundToInt(info.Location.Z),
				FMath::RoundToInt(info.Scale.X), FMath::RoundToInt(info.Scale.Y), FMath::RoundToInt(info.Scale.Z),
				FMath::RoundToInt(info.Rotation.Pitch), FMath::RoundToInt(info.Rotation.Yaw), FMath::RoundToInt(info.Rotation.Roll));
		}

		UE_LOG(LogTemp, Log, TEXT("~~~~~~~~~~~~~~~~~~  Save  ends ~~~~~~~~~~~~~~~~~~~~~~~~"));

#endif
	}

private:
	FORCEINLINE void updateBeforeSave() {

		usedBlocks.Empty();
		for (auto usedBlock : UsedBlocks)
		{
			if (usedBlock)
				usedBlocks.Add(usedBlock->ToContainer());
		}


		buildableBlocks.Empty();
		for (auto buildableBlock : BuildableBlocks)
		{
			if (buildableBlock && buildableBlock->IsValidLowLevel() && !buildableBlock->IsSystemAction())
				buildableBlocks.Add(buildableBlock->ToContainer());
		}

		check(InventoryTags && InventoryTags->IsValidLowLevel());
		inventoryTags = InventoryTags->ToContainer();

		DEBUGPrintSave();
	}

	FORCEINLINE void updateAfterLoad() {

		UsedBlocks.Empty();
		for (auto usedBlock : usedBlocks)
		{
			auto newBlock = NewObject<UBlockInfo>(this);
			newBlock->FromContainer(usedBlock);
			UsedBlocks.Add(newBlock);
		}


		BuildableBlocks = UBuildableBlockInfo::GetSystemActions();

		for (auto buildableBlock : buildableBlocks)
		{
			auto buildableBlockInfoBlock = NewObject<UBuildableBlockInfo>(this);
			buildableBlockInfoBlock->FromContainer(buildableBlock);
			BuildableBlocks.Add(buildableBlockInfoBlock);
		}

		InventoryTags = NewObject<UInventoryTags>(this);
		InventoryTags->FromContainer(inventoryTags);
	}




};


