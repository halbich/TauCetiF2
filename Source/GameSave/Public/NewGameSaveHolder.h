#pragma once

#include "Object.h"
#include "Commons/Public/Enums.h"
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
	USaveGameCarrier* getMiniLevelGameSave(bool full = false);
	USaveGameCarrier* getSuperSizeGameSave(bool full = false);

	FORCEINLINE FBlockInfo make(int32 id, FVector location, FVector blockScale, FRotator blockRotation, float health, FString name = TEXT(""))
	{
		FBlockInfo ret;
		ret.ID = id;
		ret.Location = location;
		ret.Scale = blockScale;
		ret.Rotation = blockRotation;
		ret.Health = health;
		ret.Name = name;
		return ret;
	}

	FBlockInfo make1(int32 id, float lx, float ly, float lz, float sx, float sy, float sz, float rx, float ry, float rz, float health, FString name = TEXT(""))
	{
		return make(id, FVector(lx, ly, lz), FVector(sx, sy, sz), FRotator(rx, ry, rz), health, name);
	}


	void mO(FBlockInfo& blockInfo, float currentFilling)
	{
		blockInfo.HasOxygenData = true;
		blockInfo.OxygenInfo.CurrentObjectOxygen = currentFilling;
	}

	void mE(FBlockInfo& blockInfo, float currentFilling)
	{
		blockInfo.HasElectricityData = true;
		blockInfo.ElectricityInfo.CurrentObjectEnergy = currentFilling;
	}

	void af(FBlockInfo& blockInfo, FString key, int32 value)
	{
		blockInfo.AdditionalFlags.Add(key, value);
	}

	void bd(FBlockInfo& blockInfo, FString key, FString value)
	{
		blockInfo.BlockSpecificData.Add(key, value);
	}

	void mr(FBlockInfo& blockInfo, FString guid)
	{
		blockInfo.HasRelationshipData = true; 
		FGuid id; FGuid::Parse(guid, id); 
		blockInfo.RelationshipInfo.ID = id;
	}

	void mrc(FBlockInfo& blockInfo, FString guid, uint8 type)
	{
		FRelationshipInfo rel; 
		FGuid relID; FGuid::Parse(guid, relID); rel.TargetID = relID; rel.RelationshipType = type;
		blockInfo.RelationshipInfo.Relationships.Add(rel);
	}

	FInventoryBuildableBlockInfo makeBuildable(int32 blockID, FVector blockScale)
	{
		FInventoryBuildableBlockInfo ret;
		ret.ID = blockID;
		ret.Scale = blockScale;
		return ret;
	}

	FInventoryBuildableItemBlockInfo makeInventoryBuildable(int32 blockID, FVector blockScale)
	{
		FInventoryBuildableItemBlockInfo ret;
		ret.ID = blockID;
		ret.Scale = blockScale;
		return ret;
	}

	FInventoryTags makeDefault()
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

	FWeatherState makeRandomWeatherState()
	{
		FWeatherState res;

		return res;
	}
};
