

#pragma once

#include "Object.h"
#include "Blocks/BlockInfo.h"
#include "Helpers/ArchiveHelpers.h"
#include "SaveGameCarrier.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Transient)
class TAUCETIF2_API USaveGameCarrier : public UObject
{
	GENERATED_BODY()

public:
	USaveGameCarrier();

	~USaveGameCarrier();

	static const uint8 CURRENT_VERSION;

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

	TArray<FBlockInfo> blocks;


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


	FORCEINLINE void updateBeforeSave() {

		blocks.Empty();
		for (auto usedBlock : UsedBlocks)
		{
			if (usedBlock)
				blocks.Add(usedBlock->ToContainer());
		}
	}

	FORCEINLINE void updateAfterLoad() {

		UsedBlocks.Empty();
		for (auto usedBlock : blocks)
		{
			auto newBlock = NewObject<UBlockInfo>();
			newBlock->FromContainer(usedBlock);
			UsedBlocks.Add(newBlock);
		}
	}

	FORCEINLINE UBlockInfo* make(
		FVector location,
		FVector blockScale,
		FRotator blockRotation,

		EShapeType shapeType,
		EBlockType blockType,
		EMaterialType materialType
		) {
		auto ret = NewObject<UBlockInfo>();

		ret->Location = location;
		ret->BlockScale = blockScale;
		ret->BlockRotation = blockRotation;
		ret->ShapeType = shapeType;
		ret->BlockType = blockType;
		ret->MaterialType = materialType;


		return ret;

	}

};


