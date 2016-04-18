

#pragma once

#include "Object.h"
#include "Helpers/Saving/ArchiveHelpers.h"
#include "Helpers/Saving/BlockSaveInfo.h"
#include "Blocks/Info/BlockInfo.h"
#include "Blocks/Info/BuildableBlockInfo.h"
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
	UPROPERTY()
		bool IsSystemSave;



	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		FString FullFilePath;


	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		bool ContainsFullSaveData;

	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		bool SaveLoaded;


	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		uint8 SaveFileVersion;

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FString SaveName;

	UPROPERTY(BlueprintReadOnly, Category = SaveGameCarrier)
		FDateTime SavedDate;

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FTimespan PlayedTime;

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		bool IsQuickSave;

	// game - related stuff



	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		float PartOfDay;	// interval [0,1]

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		uint8 MinBoxSize;



	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FVector PlayerPosition;

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FRotator PlayerRotation;

	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		FRotator PlayerCameraRotation;


	// blocks 
	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		TArray<UBlockInfo*> UsedBlocks;

	// Serializable array
	TArray<FBlockInfo> usedBlocks;


	// blocks 
	UPROPERTY(BlueprintReadWrite, Category = SaveGameCarrier)
		TArray<UBuildableBlockInfo*> BuildableBlocks;

	// Serializable array
	TArray<FInventoryBuildableBlockInfo> buildableBlocks;



	// functions

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static USaveGameCarrier* GetEmptyCarrier();


	void SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier, bool bFullObject = true);
	bool SaveGameDataToFile(const FString& FullFilePath);
	bool LoadGameDataFromFile(const FString& FullFilePath, bool bFullObject = true);

	bool IsSaveCompatible(const USaveGameCarrier& carrier);


	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool SaveBinary();

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		bool LoadBinary(const FString& FilePath);


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SAveGame)
		static TArray<USaveGameCarrier*> GetSaveGameInfoList();

	UFUNCTION(BlueprintCallable, Category = TCF2SAveGame)
		void GetSaveForNewGame();


private:

	void DEBUGPrintSave() {

		UE_LOG(LogTemp, Log, TEXT("~~~~~~~~~~~~~~~~~~  Save ~~~~~~~~~~~~~~~~~~~~~~~~"));

		for (auto info : usedBlocks)
		{

			UE_LOG(LogTemp, Log, TEXT("UsedBlocks.Add(make(EBlockName::%s, FVector(%d, %d, %d), FVector(%d, %d, %d), FRotator(%d, %d, %d)));"), 
				*GetEBlockNameAsString( info.ID),
				FMath::Round(info.Location.X), FMath::Round(info.Location.Y), FMath::Round(info.Location.Z),
				FMath::Round(info.Scale.X), FMath::Round(info.Scale.Y), FMath::Round(info.Scale.Z),
				FMath::Round(info.Rotation.Pitch), FMath::Round(info.Rotation.Yaw), FMath::Round(info.Rotation.Roll));
		}

		UE_LOG(LogTemp, Log, TEXT("~~~~~~~~~~~~~~~~~~  Save  ends ~~~~~~~~~~~~~~~~~~~~~~~~"));
	}



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
			if (buildableBlock && buildableBlock->IsValidLowLevel() && !buildableBlock->IsEmptyHand)
				buildableBlocks.Add(buildableBlock->ToContainer());
		}

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


		BuildableBlocks.Empty();
		BuildableBlocks.Add(NewObject<UBuildableBlockInfo>(this));
		BuildableBlocks[0]->IsEmptyHand = true;

		for (auto buildableBlock : buildableBlocks)
		{
			auto buildableBlockInfoBlock = NewObject<UBuildableBlockInfo>(this);
			buildableBlockInfoBlock->FromContainer(buildableBlock);
			BuildableBlocks.Add(buildableBlockInfoBlock);
		}
	}

	FORCEINLINE UBlockInfo* make(uint32 id, FVector location, FVector blockScale, FRotator blockRotation)
	{
		auto ret = NewObject<UBlockInfo>(this);
		ret->ID = id;
		ret->Location = location;
		ret->Scale = blockScale;
		ret->Rotation = blockRotation;
		return ret;

	}

	FORCEINLINE UBlockInfo* make(EBlockName id, FVector location, FVector blockScale, FRotator blockRotation)
	{
		return make((uint32)id, location, blockScale, blockRotation);
	}


	FORCEINLINE UBuildableBlockInfo* makeBuildable(EBlockName id, FVector blockScale)
	{
		auto ret = NewObject<UBuildableBlockInfo>(this);
		ret->ID = (int32)id;
		ret->Scale = blockScale;
		return ret;
	}



};


