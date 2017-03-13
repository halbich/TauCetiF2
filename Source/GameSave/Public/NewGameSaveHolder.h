#pragma once

#include "Object.h"
#include "Commons/Public/Enums.h"
#include "Commons/Public/LocalizationHelpers.h"
#include "SaveGameCarrier.h"
#include "Inventory/Public/InventoryTags.h"
#include "NewGameSaveHolder.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class GAMESAVE_API UNewGameSaveHolder : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
		TArray<USaveGameCarrier*> newGameSaves;

	UPROPERTY(Transient)
		USaveGameCarrier* mainMenuSave;

	UFUNCTION(BlueprintCallable, Category = TCF2SaveGame)
		TArray<USaveGameCarrier*> GetNewSaveGamesList();

	UFUNCTION(BlueprintCallable, Category = TCF2SaveGame)
		USaveGameCarrier* DEBUG_GetTestSave();

	UFUNCTION(BlueprintCallable, Category = TCF2SaveGame)
		USaveGameCarrier* GetSaveForMainMenu();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = TCF2SaveGame)
		static UNewGameSaveHolder* Instance();

private:

	void init();

	//The Function Pointer Variable Type
	//Functions take in 0 parameters and return void
	typedef USaveGameCarrier* (UNewGameSaveHolder::*FunctionPtrType)(void);

	//A static array of to Function Pointers
	FunctionPtrType fillingFunctions[(uint8)ENamedHardcodedLevel::HardcodedLevelsMax];

	USaveGameCarrier* getDefaultGameSave();
	USaveGameCarrier* getEmptyGameSave();
	USaveGameCarrier* getMainMenuSave();

	FORCEINLINE FBlockInfo make(int32 id, FVector location, FVector blockScale, FRotator blockRotation)
	{
		FBlockInfo ret;
		ret.ID = id;
		ret.Location = location;
		ret.Scale = blockScale;
		ret.Rotation = blockRotation;
		return ret;
	}

	FORCEINLINE void makeOxygen(FBlockBaseInfo& blockInfo, float currentFilling)
	{
		blockInfo.HasOxygenData = true;
		blockInfo.OxygenInfo.CurrentFillingValue = currentFilling;
	}

	FORCEINLINE FInventoryBuildableBlockInfo makeBuildable(int32 blockID, FVector blockScale)
	{
		FInventoryBuildableBlockInfo ret;
		ret.ID = blockID;
		ret.Scale = blockScale;

		ret.AddImplicitTags();
		return ret;
	}

	FORCEINLINE FInventoryBuildableItemBlockInfo makeInventoryBuildable(int32 blockID, FVector blockScale)
	{
		FInventoryBuildableItemBlockInfo ret;
		ret.ID = blockID;
		ret.Scale = blockScale;

		ret.AddImplicitTags();
		return ret;
	}

	/*FORCEINLINE FInventoryBuildableItemBlockInfo makeBuildable(int32 blockID, FVector blockScale)
	{
		FInventoryBuildableBlockInfo ret;
		ret.ID = blockID;
		ret.Scale = blockScale;

		ret.AddImplicitTags();
		return ret;
	}*/
};
