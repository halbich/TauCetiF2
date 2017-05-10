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

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SaveGameCarrier")
		bool IsSystemSave;

	UPROPERTY(Transient)
		ENamedHardcodedLevel HardcodedLevelName;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SaveGameCarrier")
		FString FullFilePath;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SaveGameCarrier")
		bool ContainsFullSaveData;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SaveGameCarrier")
		bool SaveLoaded;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SaveGameCarrier")
		uint8 SaveFileVersion;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SaveGameCarrier")
		FString SaveName;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SaveGameCarrier")
		FText SaveFullDescription;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SaveGameCarrier")
		FDateTime SavedDate;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SaveGameCarrier")
		FTimespan PlayedTime;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SaveGameCarrier")
		bool IsQuickSave;

	// game - related stuff

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SaveGameCarrier")
		float CurrentTime;	// interval [0,86400]

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SaveGameCarrier")
		uint8 MinBoxSize;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SaveGameCarrier")
		FVector PlayerPosition;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SaveGameCarrier")
		FRotator PlayerRotation;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SaveGameCarrier")
		FRotator PlayerCameraRotation;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SaveGameCarrier")
		bool PlayerUseFPSCamera;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | SaveGameCarrier")
		float PlayerHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SaveGameCarrier")
		bool IsCreativeMode;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SaveGameCarrier")
		int32 InventoryCurrentIndex;

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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | SaveGame")
		static USaveGameCarrier* GetEmptyCarrier();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | SaveGame")
		static USaveGameCarrier* GetQuickSaveCarrier(TArray<FText>& errorList);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SaveGame")
		bool DeleteSaveFile();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SaveGame")
		bool SaveBinary(TArray<FText>& errorList);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SaveGame")
		bool LoadBinary(const FString& FilePath, TArray<FText>& errorList);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | SaveGame")
		static TArray<USaveGameCarrier*> GetSaveGameInfoList(TArray<FText>& errorList);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SaveGame")
		bool IsSaveNameValid();

private:

	void SaveLoadData(FArchive& Ar, USaveGameCarrier& carrier, TArray<FText>& errorList, bool bFullObject = true);
	bool SaveGameDataToFile(const FString& FullFilePath, TArray<FText>& errorList);
	bool LoadGameDataFromFile(const FString& FullFilePath, TArray<FText>& errorList, bool bFullObject = true);

	void DEBUGPrintSave() {
#if WITH_EDITOR
		UE_LOG(LogTemp, Log, TEXT("~~~~~~~~~~~~~~~~~~  Save ~~~~~~~~~~~~~~~~~~~~~~~~"));

		int32 count = 0;
		for (auto info : usedBlocks)
		{
			TArray<FStringFormatArg> args;
			args.Add(FStringFormatArg(count));
			args.Add(FStringFormatArg(info.ID));
			args.Add(FStringFormatArg(FMath::RoundToInt(info.Location.X))); args.Add(FStringFormatArg(FMath::RoundToInt(info.Location.Y))); args.Add(FStringFormatArg(FMath::RoundToInt(info.Location.Z)));
			args.Add(FStringFormatArg(FMath::RoundToInt(info.Scale.X))); args.Add(FStringFormatArg(FMath::RoundToInt(info.Scale.Y))); args.Add(FStringFormatArg(FMath::RoundToInt(info.Scale.Z)));
			args.Add(FStringFormatArg(FMath::RoundToInt(info.Rotation.Pitch))); args.Add(FStringFormatArg(FMath::RoundToInt(info.Rotation.Yaw))); args.Add(FStringFormatArg(FMath::RoundToInt(info.Rotation.Roll)));
			args.Add(FStringFormatArg(info.Health));
			args.Add(FStringFormatArg(*info.Name));
			auto baseStr = FString::Format(TEXT("auto b_{0} = make1({1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, \"{12}\");"), args);

			for (auto fl : info.AdditionalFlags)
			{
				TArray<FStringFormatArg> elArgs;
				elArgs.Add(FStringFormatArg(count));
				elArgs.Add(FStringFormatArg(fl.Key));
				elArgs.Add(FStringFormatArg(fl.Value));
				baseStr += FString::Format(TEXT("af(b_{0}, TEXT(\"{1}\"), {2});"), elArgs);
			}

			//if (info.AdditionalFlags.Num() > 0)
			//{
			//	UE_LOG(LogTemp, Log, TEXT("%s"), *baseStr);
			//	baseStr.Empty();
			//}

			for (auto fl : info.BlockSpecificData)
			{
				TArray<FStringFormatArg> elArgs;
				elArgs.Add(FStringFormatArg(count));
				elArgs.Add(FStringFormatArg(fl.Key));
				elArgs.Add(FStringFormatArg(fl.Value));
				baseStr += FString::Format(TEXT("bd(b_{0}, TEXT(\"{1}\"), TEXT(\"{2}\"));"), elArgs);
			}

			//if (info.BlockSpecificData.Num() > 0)
			//{
			//	UE_LOG(LogTemp, Log, TEXT("%s"), *baseStr);
			//	baseStr.Empty();
			//}

			if (info.HasElectricityData)
			{
				TArray<FStringFormatArg> elArgs;
				elArgs.Add(FStringFormatArg(count));
				elArgs.Add(FStringFormatArg(info.ElectricityInfo.CurrentObjectEnergy));
				baseStr += FString::Format(TEXT("mE(b_{0}, {1});"), elArgs);
			}

			if (info.HasOxygenData)
			{
				TArray<FStringFormatArg> oxArgs;
				oxArgs.Add(FStringFormatArg(count));
				oxArgs.Add(FStringFormatArg(info.OxygenInfo.CurrentObjectOxygen));
				baseStr += FString::Format(TEXT("mO(b_{0}, {1});"), oxArgs);
			}

			if (info.HasRelationshipData)
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *baseStr);
				baseStr.Empty();

				TArray<FStringFormatArg> relArgs;
				relArgs.Add(FStringFormatArg(count));
				relArgs.Add(FStringFormatArg(info.RelationshipInfo.ID.ToString()));
				baseStr += FString::Format(TEXT("mr(b_{0}, \"{1}\");"), relArgs);

				for (auto relation : info.RelationshipInfo.Relationships)
				{
					TArray<FStringFormatArg> relItemArgs;
					relItemArgs.Add(FStringFormatArg(count));
					relItemArgs.Add(FStringFormatArg(relation.TargetID.ToString()));
					relItemArgs.Add(FStringFormatArg(relation.RelationshipType));

					auto commands = TEXT("mrc(b_{0}, \"{1}\", {2});");
					baseStr += FString::Format(commands, relItemArgs);
				}
			}

			UE_LOG(LogTemp, Log, TEXT("%s UsedBlocks->Add(b_%d);"), *baseStr, count++);
		}

		UE_LOG(LogTemp, Log, TEXT("c->PlayerPosition = FVector(%d, %d, %d);"),
			FMath::RoundToInt(PlayerPosition.X), FMath::RoundToInt(PlayerPosition.Y), FMath::RoundToInt(PlayerPosition.Z)
		);

		UE_LOG(LogTemp, Log, TEXT("c->PlayerRotation = FRotator(%d, %d, %d);"),
			FMath::RoundToInt(PlayerRotation.Pitch), FMath::RoundToInt(PlayerRotation.Yaw), FMath::RoundToInt(PlayerRotation.Roll)
		);

		UE_LOG(LogTemp, Log, TEXT("c->PlayerCameraRotation = FRotator(%s, %s, %s);"),
			*FString::SanitizeFloat(PlayerCameraRotation.Pitch), *FString::SanitizeFloat(PlayerCameraRotation.Yaw), *FString::SanitizeFloat(PlayerCameraRotation.Roll)
		);

		UE_LOG(LogTemp, Log, TEXT("~~~~~~~~~~~~~~~~~~  Save  ends ~~~~~~~~~~~~~~~~~~~~~~~~"));

#endif
	}
};
