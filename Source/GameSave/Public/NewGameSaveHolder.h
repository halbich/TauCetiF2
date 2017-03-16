#pragma once

#include "Object.h"
#include "Commons/Public/Enums.h"
#include "Commons/Public/LocalizationHelpers.h"
#include "SaveGameCarrier.h"
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

	UFUNCTION(BlueprintCallable, Category = TCF2SaveGame)
		USaveGameCarrier* GetSaveByPath(FString path);

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

	UInventoryTags* UInventoryTags::GetDefault()
	{
		// TODO check aginst gamesave, ii could be removed from here and elt this method live only in gamesave

		auto res = NewObject<UInventoryTags>();
		for (int32 i = 0; i < GameDefinitions::DefaultInventoryTagGroupsCount; i++)
		{
			auto itm = NewObject<UInventoryTagGroup>();
			if (i == 0)
			{
				itm->IsGroupEnabled = true;
				res->CurrentActiveIndex = 0;
				itm->GroupType = EInventoryGroupType::Inventory;
			}
			if (i == 1)
			{
				itm->IsGroupEnabled = true;
				res->CurrentActiveIndex = 1;
			}

			itm->GroupName = FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.InventoryTags.Default_Group_Name", "Skupina {0}"), FText::AsNumber(i + 1)).ToString();
			res->InventoryGroupList.Add(itm);
		}

		return res;
	}
};
