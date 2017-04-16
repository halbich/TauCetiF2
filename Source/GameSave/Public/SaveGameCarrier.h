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

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | SaveGameCarrier")
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
			auto baseStr = FString::Format(TEXT("auto b_{0} = make({1}, FVector({2}, {3}, {4}), FVector({5}, {6}, {7}), FRotator({8}, {9}, {10}), {1}, \"{12}\"); "), args);


			if (info.HasElectricityData)
			{
				TArray<FStringFormatArg> elArgs;
				elArgs.Add(FStringFormatArg(count));
				elArgs.Add(FStringFormatArg(info.ElectricityInfo.CurrentObjectEnergy));
				baseStr += FString::Format(TEXT("b_{0}.HasElectricityData = true; b_{0}.ElectricityInfo.CurrentObjectEnergy = {1}; "), elArgs);

			}

			if (info.HasOxygenData)
			{
				TArray<FStringFormatArg> oxArgs;
				oxArgs.Add(FStringFormatArg(count));
				oxArgs.Add(FStringFormatArg(info.OxygenInfo.CurrentFillingValue));
				baseStr += FString::Format(TEXT("b_{0}.HasOxygenData = true; b_{0}.OxygenInfo.CurrentFillingValue = {1}; "), oxArgs);
			}

			if (info.HasRelationshipData)
			{

				UE_LOG(LogTemp, Log, TEXT("%s"), *baseStr);
				baseStr.Empty();

				TArray<FStringFormatArg> relArgs;
				relArgs.Add(FStringFormatArg(count));
				relArgs.Add(FStringFormatArg(info.RelationshipInfo.ID.ToString()));
				baseStr += FString::Format(TEXT("b_{0}.HasRelationshipData = true; FGuid b_{0}_rel_id; FGuid::Parse(\"{1}\", b_{0}_rel_id); b_{0}.RelationshipInfo.ID = b_{0}_rel_id;"), relArgs);


				int32 relCount = 0;
				for (auto relation : info.RelationshipInfo.Relationships)
				{

					UE_LOG(LogTemp, Log, TEXT("%s"), *baseStr);
					baseStr.Empty();

					TArray<FStringFormatArg> relItemArgs;
					relItemArgs.Add(FStringFormatArg(count));
					relItemArgs.Add(FStringFormatArg(relCount));
					relItemArgs.Add(FStringFormatArg(relation.TargetID.ToString()));
					relItemArgs.Add(FStringFormatArg(relation.RelationshipType));

					auto commands = TEXT("FRelationshipInfo b_{0}_rel{1}; FGuid b_{0}_rel{1}_id; FGuid::Parse(\"{2}\", b_{0}_rel{1}_id); b_{0}_rel{1}.TargetID = b_{0}_rel{1}_id; b_{0}_rel{1}.RelationshipType = {3};");
					baseStr += FString::Format(commands, relItemArgs);

					UE_LOG(LogTemp, Log, TEXT("%s"), *baseStr);
					baseStr.Empty();

					TArray<FStringFormatArg> relItemArgs1;
					relItemArgs1.Add(FStringFormatArg(count));
					relItemArgs1.Add(FStringFormatArg(relCount));

					auto commands1 = TEXT("b_{0}.RelationshipInfo.Relationships.Add(b_{0}_rel{1});");
					baseStr += FString::Format(commands1, relItemArgs1);


					++relCount;
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

		UE_LOG(LogTemp, Log, TEXT("~~~~~~~~~~~~~~~~~~  Save  ends ~~~~~~~~~~~~~~~~~~~~~~~~"));

#endif
	}
};
