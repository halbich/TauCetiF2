#pragma once

#include "Object.h"
#include "Commons/Public/Enums.h"
#include "Commons/Public/LocalizationHelpers.h"
#include "SaveGameCarrier.h"
#include "Inventory/FInventoryTags.h"
#include "Inventory/FInventoryTagGroup.h"
#include "NewGameSaveHolder.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class GAMESAVE_API UNewGameSaveHolder : public UObject
{
	GENERATED_BODY()

private:
	//The Function Pointer Variable Type
	//Functions take in 0 parameters and return void
	typedef USaveGameCarrier* (UNewGameSaveHolder::*FunctionPtrType)(bool);

public:
	UNewGameSaveHolder();
	~UNewGameSaveHolder();

	TMap<ENamedHardcodedLevel, FunctionPtrType> systemSaves;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SaveGame")
		TArray<USaveGameCarrier*> GetNewSaveGamesList();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SaveGame")
		USaveGameCarrier* GetSaveForMainMenu();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | SaveGame")
		static UNewGameSaveHolder* Instance();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | SaveGame")
		USaveGameCarrier* GetSaveByPath(FString path);

private:

	USaveGameCarrier* getDefaultGameSave(bool full = false);
	USaveGameCarrier* getEmptyGameSave(bool full = false);
	USaveGameCarrier* getMainMenuSave(bool full = false);

	FORCEINLINE FBlockInfo make(int32 id, FVector location, FVector blockScale, FRotator blockRotation, float health)
	{
		FBlockInfo ret;
		ret.ID = id;
		ret.Location = location;
		ret.Scale = blockScale;
		ret.Rotation = blockRotation;
		ret.Health = health;
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

	FORCEINLINE FInventoryTags makeDefault()
	{
		FInventoryTags res;
		for (int32 i = 0; i < GameDefinitions::DefaultInventoryTagGroupsCount; i++)
		{
			FInventoryTagGroup itm;
			if (i == 0)
			{
				itm.IsGroupEnabled = true;
				res.CurrentActiveIndex = 0;
				itm.GroupType = (uint8)EInventoryGroupType::Inventory;
			}
			if (i == 1)
			{
				itm.IsGroupEnabled = true;
				res.CurrentActiveIndex = 1;
			}

			itm.GroupName = FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.InventoryTags.Default_Group_Name", "Skupina {0}"), FText::AsNumber(i + 1)).ToString();
			res.InventoryGroupList.Add(itm);
		}

		return res;
	}

	FORCEINLINE FWeatherState makeRandomWeatherState()
	{
		FWeatherState res;

		return res;
	}
};
