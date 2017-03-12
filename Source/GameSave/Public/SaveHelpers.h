#pragma once



#include "Object.h"
#include "BlockSaveInfo.h"
#include "Blocks/Public/Info/BlockBaseInfo.h"
#include "Blocks/Public/Info/BlockInfo.h"
#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "Blocks/Public/Info/InventoryBuildableBlockInfo.h"
#include "Inventory/Public/TagGroup.h"
#include "Inventory/FTagGroup.h"
#include "Inventory/Public/InventoryTags.h"
#include "Inventory/FInventoryTags.h"
#include "Inventory/Public/InventoryTagGroup.h"
#include "Inventory/FInventoryTagGroup.h"

#include "Blocks/Public/Info/Components/BlockWithOxygenInfo.h"
#include "BlockComponents/OxygenComponentInfo.h"
#include "Blocks/Public/Info/Components/BlockWithElectricityInfo.h"
#include "BlockComponents/ElectricityComponentInfo.h"


#include "SaveHelpers.generated.h"

/**
 *
 */
UCLASS()
class GAMESAVE_API USaveHelpers : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static TArray<FString> GetAllSaveGameSlots();

	static FORCEINLINE FString USaveHelpers::GetCleanSaveFileName(const FString& worldName, const FDateTime& saveTime) {
		auto name = worldName.Replace(TEXT(" "), TEXT("_"));
		name.ToLowerInline();
		auto date = saveTime.ToString(TEXT("%d_%m_%Y_%H_%M_%S"));

		return FString::Printf(TEXT("%s_%s"), *name, *date);
	}


private:
	FORCEINLINE static void FromBaseContainer(UBlockBaseInfo* info, const FBlockBaseInfo& block) {
		info->ID = block.ID;
		info->Scale = block.Scale;
		info->Name = block.Name;
		info->AdditionalFlags = block.AdditionalFlags;
	}

	FORCEINLINE static void ToBaseContainer(FBlockBaseInfo& block, const UBlockBaseInfo* info) {
		block.ID = info->ID;
		block.Scale = info->Scale;
		block.Name = info->Name;
		block.AdditionalFlags = info->AdditionalFlags;
	}

	FORCEINLINE static void FromContainer(UBlockWithOxygenInfo* info, const FOxygenComponentInfo& block) {
		info->CurrentFillingValue = block.CurrentFillingValue;
	}

	FORCEINLINE static void ToContainer(FOxygenComponentInfo& block, const UBlockWithOxygenInfo* info) {
		block.CurrentFillingValue = info->CurrentFillingValue;
	}

	FORCEINLINE static void FromContainer(UBlockWithElectricityInfo* info, const FElectricityComponentInfo& block) {
	}

	FORCEINLINE static void ToContainer(FElectricityComponentInfo& block, const UBlockWithElectricityInfo* info) {
	}

public:
	 static void FromContainer(UBlockInfo* info, const FBlockInfo& block) {
		FromBaseContainer(info, block);
		info->Location = block.Location;
		info->Rotation = block.Rotation;

		if (block.HasOxygenData)
		{
			info->OxygenInfo = NewObject<UBlockWithOxygenInfo>();
			FromContainer(info->OxygenInfo, block.OxygenInfo);
		}

		if (block.HasElectricityData)
		{
			info->ElectricityInfo = NewObject<UBlockWithElectricityInfo>();
			FromContainer(info->ElectricityInfo, block.ElectricityInfo);
		}

	}

	 static void ToContainer(FBlockInfo& block, const UBlockInfo* info) {
		ToBaseContainer(block, info);
		block.Location = info->Location;
		block.Rotation = info->Rotation;

		if (info->OxygenInfo && info->OxygenInfo->IsValidLowLevel())
		{
			block.HasOxygenData = true;
			ToContainer(block.OxygenInfo, info->OxygenInfo);
		}

		if (info->ElectricityInfo && info->ElectricityInfo->IsValidLowLevel())
		{
			block.HasElectricityData = true;
			ToContainer(block.ElectricityInfo, info->ElectricityInfo);
		}
	}

	FORCEINLINE static void FromContainer(UBuildableBlockInfo* info, const FInventoryBuildableBlockInfo& block) {
		FromBaseContainer(info, block);
		info->Tags = block.Tags;
	}

	FORCEINLINE static void ToContainer(FInventoryBuildableBlockInfo& block, const UBuildableBlockInfo* info) {
		ToBaseContainer(block, info);
		block.Tags = info->Tags;
	}

	FORCEINLINE static void FromInventoryContainer(UInventoryBuildableBlockInfo* info, const FInventoryBuildableItemBlockInfo& block) {
		FromContainer(info, block);
		info->Tags = block.Tags;
	}

	FORCEINLINE static void ToInventoryContainer(FInventoryBuildableItemBlockInfo& block, const UInventoryBuildableBlockInfo* info) {
		ToContainer(block, info);
		block.Tags = info->Tags;
	}
	

private:
	FORCEINLINE static void FromContainer(UTagGroup* grp, const FTagGroup& group) {
		grp->GroupName = group.GroupName;
		grp->LetVisibleAll = group.LetVisibleAll;
		grp->Tags = TArray<FString>(group.Tags);
	}

	FORCEINLINE static void ToContainer(FTagGroup& group, const UTagGroup* grp) {
		group.GroupName = grp->GroupName;
		group.LetVisibleAll = grp->LetVisibleAll;
		group.Tags = TArray<FString>(grp->Tags);
	}

	FORCEINLINE static void FromContainer(UInventoryTagGroup* tagGroup, const FInventoryTagGroup& invTagGroup) {
		tagGroup->GroupName = invTagGroup.GroupName;
		tagGroup->IsGroupEnabled = invTagGroup.IsGroupEnabled;
		tagGroup->GroupType = (EInventoryGroupType)invTagGroup.GroupType;

		for (auto groupList : invTagGroup.GroupList)
		{
			auto gl = NewObject<UTagGroup>();
			FromContainer(gl, groupList);
			tagGroup->GroupList.Add(gl);
		}
	}

	FORCEINLINE static void ToContainer(FInventoryTagGroup& invTagGroup, const UInventoryTagGroup* tagGroup) {
		invTagGroup.GroupName = tagGroup->GroupName;
		invTagGroup.IsGroupEnabled = tagGroup->IsGroupEnabled;
		invTagGroup.GroupType = (uint8)tagGroup->GroupType;
		invTagGroup.GroupList.Empty();

		for (const auto tagGrp : tagGroup->GroupList)
		{
			FTagGroup grp;
			ToContainer(grp, tagGrp);
			invTagGroup.GroupList.Add(grp);
		}
	}

public:
	FORCEINLINE static void FromContainer(UInventoryTags* tags, const FInventoryTags& invTags) {
		tags->CurrentActiveIndex = invTags.CurrentActiveIndex;

		for (auto invTagGroup : invTags.InventoryGroupList)
		{
			auto igl = NewObject<UInventoryTagGroup>();
			FromContainer(igl, invTagGroup);
			tags->InventoryGroupList.Add(igl);
		}
	}

	FORCEINLINE static void ToContainer(FInventoryTags& invTags, const UInventoryTags* tags) {
		invTags.CurrentActiveIndex = tags->CurrentActiveIndex;
		invTags.InventoryGroupList.Empty();

		for (auto group : tags->InventoryGroupList)
		{
			FInventoryTagGroup grp;
			ToContainer(grp, group);
			invTags.InventoryGroupList.Add(grp);
		}
	}
};
#pragma region Loading

FORCEINLINE TArray<FBlockInfo>& operator >> (TArray<FBlockInfo>& blockArray, TArray<UBlockInfo*>& blockObjectArray)
{
	for (auto block : blockArray)
	{
		auto NewItem = NewObject<UBlockInfo>();
		USaveHelpers::FromContainer(NewItem, block);
		blockObjectArray.Add(NewItem);
	}
	return blockArray;
}

FORCEINLINE TArray<FInventoryBuildableBlockInfo>& operator >> (TArray<FInventoryBuildableBlockInfo>& blockArray, TArray<UBuildableBlockInfo*>& blockObjectArray)
{
	for (auto block : blockArray)
	{
		auto NewItem = NewObject<UBuildableBlockInfo>();
		USaveHelpers::FromContainer(NewItem, block);

		
		blockObjectArray.Add(NewItem);
	}
	return blockArray;
}

FORCEINLINE FInventoryTags& operator >> (FInventoryTags& invTags, UInventoryTags* tags)
{
	USaveHelpers::FromContainer(tags, invTags);
	return invTags;
}

FORCEINLINE TArray<FInventoryBuildableItemBlockInfo>& operator >> (TArray<FInventoryBuildableItemBlockInfo>& blockArray, TArray<UInventoryBuildableBlockInfo*>& blockObjectArray)
{
	for (auto block : blockArray)
	{
		auto NewItem = NewObject<UInventoryBuildableBlockInfo>();
		USaveHelpers::FromInventoryContainer(NewItem, block);


		blockObjectArray.Add(NewItem);
	}
	return blockArray;
}


#pragma endregion

#pragma region Saving

FORCEINLINE TArray<FBlockInfo>& operator<<(TArray<FBlockInfo>& blockArray, TArray<UBlockInfo*>& blockObjectArray)
{
	for (auto usedBlock : blockObjectArray)
	{
		FBlockInfo block;
		USaveHelpers::ToContainer(block, usedBlock);
		blockArray.Add(block);
	}
	return blockArray;
}

FORCEINLINE TArray<FInventoryBuildableBlockInfo>& operator<<(TArray<FInventoryBuildableBlockInfo>& blockArray, TArray<UBuildableBlockInfo*>& blockObjectArray)
{
	for (auto usedBlock : blockObjectArray)
	{
		FInventoryBuildableBlockInfo block;
		USaveHelpers::ToContainer(block, usedBlock);
		blockArray.Add(block);
	}
	return blockArray;
}

FORCEINLINE FInventoryTags& operator << (FInventoryTags& invTags, UInventoryTags* tags)
{
	USaveHelpers::ToContainer(invTags, tags);
	return invTags;
}

FORCEINLINE TArray<FInventoryBuildableItemBlockInfo>& operator<<(TArray<FInventoryBuildableItemBlockInfo>& blockArray, TArray<UInventoryBuildableBlockInfo*>& blockObjectArray)
{
	for (auto usedBlock : blockObjectArray)
	{
		FInventoryBuildableItemBlockInfo block;
		USaveHelpers::ToInventoryContainer(block, usedBlock);
		blockArray.Add(block);
	}
	return blockArray;
}



#pragma endregion