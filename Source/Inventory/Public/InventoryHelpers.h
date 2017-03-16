#pragma once

#include "GameSave/Public/SaveGameCarrier.h"
#include "InventoryTags.h"
#include "Blocks/Public/Helpers/BlockHelpers.h"

namespace InventoryHelpers {


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




	FORCEINLINE static void FromContainer(UBuildableBlockInfo* info, const FInventoryBuildableBlockInfo& block) {
		BlockSavingHelpers::FromBaseContainer(info, block);
		info->Tags = block.Tags;
	}

	FORCEINLINE static void ToContainer(FInventoryBuildableBlockInfo& block, const UBuildableBlockInfo* info) {
		BlockSavingHelpers::ToBaseContainer(block, info);
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

	TArray<UBuildableBlockInfo*> GetBuildableBlockData(USaveGameCarrier* carrier)
	{
		TArray<UBuildableBlockInfo*> result;

		for (auto block : carrier->buildableBlocks)
		{
			auto NewItem = NewObject<UBuildableBlockInfo>();
			FromContainer(NewItem, block);

			result.Add(NewItem);
		}

		return result;
	}

	TArray<UInventoryBuildableBlockInfo*> GetInventoryBuildableBlockData(USaveGameCarrier* carrier)
	{
		TArray<UInventoryBuildableBlockInfo*> result;
		for (auto block : carrier->inventoryBuildableBlocks)
		{
			auto NewItem = NewObject<UInventoryBuildableBlockInfo>();
			FromInventoryContainer(NewItem, block);
			NewItem->UpdateDisplayValue();
			result.Add(NewItem);
		}
		return result;
	}

	UInventoryTags* GetInventoryTags(USaveGameCarrier* carrier)
	{
		auto result = NewObject<UInventoryTags>();
		FromContainer(result, carrier->inventoryTags);

		return result;
	}

	void SetInventoryTags(USaveGameCarrier* carrier, UInventoryTags* InventoryTags)
	{
		ToContainer(carrier->inventoryTags, InventoryTags);
	}

	void SetBuildableBlocks(USaveGameCarrier* carrier, TArray<UBuildableBlockInfo*>& BuildableBlocks)
	{
		carrier->buildableBlocks.Empty();

		for (auto usedBlock : BuildableBlocks)
		{
			FInventoryBuildableBlockInfo block;
			ToContainer(block, usedBlock);
			carrier->buildableBlocks.Add(block);
		}

	}

	void SetInventoryBuildableBlocks(USaveGameCarrier* carrier, TArray<UInventoryBuildableBlockInfo*>& InventoryBlocks)
	{
		carrier->inventoryBuildableBlocks.Empty();
		for (auto usedBlock : InventoryBlocks)
		{
			FInventoryBuildableItemBlockInfo  block;
			ToInventoryContainer(block, usedBlock);
			carrier->buildableBlocks.Add(block);
		}
	}




}