#pragma once

#include "Commons/Public/Enums.h"
#include "GameSave/Public/SaveGameCarrier.h"
#include "GameSave/Public/BlockSaveInfo.h"
#include "Blocks/Public/Helpers/BlockHelpers.h"

#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "Blocks/Public/Info/InventoryBuildableBlockInfo.h"

#include "InventoryTags.h"
#include "TagGroup.h"
#include "InventoryTagGroup.h"

#include "GameSave/Public/Inventory/FTagGroup.h"
#include "GameSave/Public/Inventory/FInventoryTagGroup.h"
#include "GameSave/Public/Inventory/FInventoryTags.h"

namespace InventoryHelpers {
	static void FromTagGroupContainer(UTagGroup* grp, FTagGroup& group) {
		grp->GroupName = group.GroupName;
		grp->LetVisibleAll = group.LetVisibleAll;
		grp->Tags = TArray<FString>(group.Tags);
	}

	static void ToTagGroupContainer(FTagGroup& group, UTagGroup* grp) {
		group.GroupName = grp->GroupName;
		group.LetVisibleAll = grp->LetVisibleAll;
		group.Tags = TArray<FString>(grp->Tags);
	}

	static void FromInvTagGroupContainer(UInventoryTagGroup* tagGroup, FInventoryTagGroup& invTagGroup) {
		tagGroup->GroupName = invTagGroup.GroupName;
		tagGroup->IsGroupEnabled = invTagGroup.IsGroupEnabled;
		tagGroup->GroupType = (EInventoryGroupType)invTagGroup.GroupType;

		for (auto groupList : invTagGroup.GroupList)
		{
			auto gl = NewObject<UTagGroup>();
			FromTagGroupContainer(gl, groupList);
			tagGroup->GroupList.Add(gl);
		}
	}

	static void ToInvTagGroupContainer(FInventoryTagGroup& invTagGroup, UInventoryTagGroup* tagGroup) {
		invTagGroup.GroupName = tagGroup->GroupName;
		invTagGroup.IsGroupEnabled = tagGroup->IsGroupEnabled;
		invTagGroup.GroupType = (uint8)tagGroup->GroupType;
		invTagGroup.GroupList.Empty();

		for (auto tagGrp : tagGroup->GroupList)
		{
			FTagGroup grp;
			ToTagGroupContainer(grp, tagGrp);
			invTagGroup.GroupList.Add(grp);
		}
	}

	static void FromInventoryContainer(UInventoryTags* tags, FInventoryTags& invTags) {
		tags->CurrentActiveIndex = invTags.CurrentActiveIndex;

		for (auto invTagGroup : invTags.InventoryGroupList)
		{
			auto igl = NewObject<UInventoryTagGroup>();
			FromInvTagGroupContainer(igl, invTagGroup);
			tags->InventoryGroupList.Add(igl);
		}
	}

	static void ToInventoryContainer(FInventoryTags& invTags, UInventoryTags* tags) {
		invTags.CurrentActiveIndex = tags->CurrentActiveIndex;
		invTags.InventoryGroupList.Empty();

		for (auto group : tags->InventoryGroupList)
		{
			FInventoryTagGroup grp;
			ToInvTagGroupContainer(grp, group);
			invTags.InventoryGroupList.Add(grp);
		}
	}

	static void FromBuildableContainer(UBuildableBlockInfo* info, FInventoryBuildableBlockInfo& block) {
		BlockSavingHelpers::FromBaseContainer(info, block);
		info->Tags = block.Tags;
	}

	static void ToBuildableContainer(FInventoryBuildableBlockInfo& block, UBuildableBlockInfo* info) {
		BlockSavingHelpers::ToBaseContainer(block, info);
		block.Tags = info->Tags;
	}

	static void FromInvBuildableContainer(UInventoryBuildableBlockInfo* info, FInventoryBuildableItemBlockInfo& block) {
		FromBuildableContainer(info, block);
	}

	static void ToInvBuildableContainer(FInventoryBuildableItemBlockInfo& block, UInventoryBuildableBlockInfo* info) {
		ToBuildableContainer(block, info);
	}

	static void SetInventoryTags(USaveGameCarrier* carrier, UInventoryTags* InventoryTags)
	{
		ToInventoryContainer(carrier->inventoryTags, InventoryTags);
	}

	static UInventoryTags* GetInventoryTags(USaveGameCarrier* carrier)
	{
		auto result = NewObject<UInventoryTags>();
		FromInventoryContainer(result, carrier->inventoryTags);
		return result;
	}

	static void SetBuildableBlocks(USaveGameCarrier* carrier, TArray<UBuildableBlockInfo*>& BuildableBlocks)
	{
		carrier->buildableBlocks.Empty();

		for (auto usedBlock : BuildableBlocks)
		{
			FInventoryBuildableBlockInfo block;
			ToBuildableContainer(block, usedBlock);
			carrier->buildableBlocks.Add(block);
		}
	}

	static TArray<UBuildableBlockInfo*> GetBuildableBlocks(USaveGameCarrier* carrier)
	{
		TArray<UBuildableBlockInfo*> result;

		for (auto block : carrier->buildableBlocks)
		{
			auto NewItem = NewObject<UBuildableBlockInfo>();
			FromBuildableContainer(NewItem, block);
			result.Add(NewItem);
		}
		return result;
	}

	static void SetInventoryBuildableBlocks(USaveGameCarrier* carrier, TArray<UInventoryBuildableBlockInfo*>& InventoryBlocks)
	{
		carrier->inventoryBuildableBlocks.Empty();
		for (auto usedBlock : InventoryBlocks)
		{
			FInventoryBuildableItemBlockInfo  block;
			ToInvBuildableContainer(block, usedBlock);
			carrier->inventoryBuildableBlocks.Add(block);
		}
	}

	static TArray<UInventoryBuildableBlockInfo*> GetInventoryBuildableBlocks(USaveGameCarrier* carrier)
	{
		TArray<UInventoryBuildableBlockInfo*> result;
		for (auto block : carrier->inventoryBuildableBlocks)
		{
			auto NewItem = NewObject<UInventoryBuildableBlockInfo>();
			FromInvBuildableContainer(NewItem, block);
			NewItem->UpdateDisplayValue();
			result.Add(NewItem);
		}
		return result;
	}
}