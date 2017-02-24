#pragma once

#include "Object.h"
#include "BlockSaveInfo.h"
#include "Blocks/Public/Info/BlockBaseInfo.h"
#include "Blocks/Public/Info/BlockInfo.h"
#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "Inventory/Public/TagGroup.h"
#include "Inventory/FTagGroup.h"
#include "Inventory/Public/InventoryTags.h"
#include "Inventory/FInventoryTags.h"
#include "Inventory/Public/InventoryTagGroup.h"
#include "Inventory/FInventoryTagGroup.h"
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

	static FInventoryTags MakeFromTags(UInventoryTags* inventoryTagsObject);
};

#pragma region Loading

FORCEINLINE void FromBaseContainer(UBlockBaseInfo* info, const FBlockBaseInfo& block) {
	info->ID = block.ID;
	info->Scale = block.Scale;
	info->Name = block.Name;
	info->AdditionalFlags = block.AdditionalFlags;
}

FORCEINLINE void FromContainer(UBlockInfo* info, const FBlockInfo& block) {
	FromBaseContainer(info, block);
	info->Location = block.Location;
	info->Rotation = block.Rotation;
}

FORCEINLINE void FromContainer(UBuildableBlockInfo* info, const FInventoryBuildableBlockInfo& block) {
	FromBaseContainer(info, block);
	info->Tags = block.Tags;
}

FORCEINLINE TArray<FBlockInfo>& operator >> (TArray<FBlockInfo>& blockArray, TArray<UBlockInfo*>& blockObjectArray)
{
	for (auto block : blockArray)
	{
		auto NewItem = NewObject<UBlockInfo>();
		FromContainer(NewItem, block);
		blockObjectArray.Add(NewItem);
	}
	return blockArray;
}

FORCEINLINE TArray<FInventoryBuildableBlockInfo>& operator >> (TArray<FInventoryBuildableBlockInfo>& blockArray, TArray<UBuildableBlockInfo*>& blockObjectArray)
{
	for (auto block : blockArray)
	{
		auto NewItem = NewObject<UBuildableBlockInfo>();
		FromContainer(NewItem, block);
		blockObjectArray.Add(NewItem);
	}
	return blockArray;
}

#pragma endregion

//FORCEINLINE FBlockBaseInfo ToBaseContainer() {
//	FBlockBaseInfo result;
//	result.ID = ID;
//	result.Scale = Scale;
//	result.Name = Name;
//	result.AdditionalFlags = AdditionalFlags;
//	return result;
//}

//FORCEINLINE FBlockInfo ToContainer() {
//	FBlockInfo result = ToBaseContainer();
//	result.Location = Location;
//	result.Rotation = Rotation;
//	return result;
//}

//FORCEINLINE FInventoryBuildableBlockInfo ToContainer() {
//	FInventoryBuildableBlockInfo result; // = ToBaseContainer();
//	//TODO
//	result.Tags = Tags;
//	return result;
//}

FORCEINLINE TArray<FBlockInfo>& operator<<(TArray<FBlockInfo>& blockArray, TArray<UBlockInfo*>& blockObjectArray)
{
	for (auto usedBlock : blockObjectArray)
	{
		/*if (usedBlock)
			blockArray.Add(usedBlock->ToContainer());*/
	}
	/*Ar << block.ID;
	Ar << block.Scale;
	Ar << block.Name;
	Ar << block.AdditionalFlags;*/
	return blockArray;
}

FORCEINLINE TArray<FInventoryBuildableBlockInfo>& operator<<(TArray<FInventoryBuildableBlockInfo>& blockArray, TArray<UBuildableBlockInfo*>& blockObjectArray)
{
	for (auto usedBlock : blockObjectArray)
	{
		/*if (usedBlock)
			blockArray.Add(usedBlock->ToContainer());*/
	}
	//for (auto buildableBlock : BuildableBlocks)
	//{
	//	if (buildableBlock && buildableBlock->IsValidLowLevel() && !buildableBlock->IsSystemAction())
	//		buildableBlocks.Add(buildableBlock->ToContainer());
	//}

	/*Ar << block.ID;
	Ar << block.Scale;
	Ar << block.Name;
	Ar << block.AdditionalFlags;*/
	return blockArray;
}


/*FORCEINLINE FInventoryTags ToContainer() {
FInventoryTags result;
result.CurrentActiveIndex = CurrentActiveIndex;

for (auto invTagGroup : InventoryGroupList)
{
if (invTagGroup && invTagGroup->IsValidLowLevel())
result.InventoryGroupList.Add(invTagGroup->ToContainer());
}

return result;
}
*/
FORCEINLINE void FromContainer(UTagGroup* grp, const FTagGroup& group) {
	grp->GroupName = group.GroupName;
	grp->Tags = TArray<FString>(group.Tags);
}

FORCEINLINE void FromContainer(UInventoryTagGroup* tagGroup, const FInventoryTagGroup& invTagGroup) {
	tagGroup->Name = invTagGroup.Name;
	tagGroup->IsEnabled = invTagGroup.IsEnabled;

	for (auto groupList : invTagGroup.GroupList)
	{
		auto gl = NewObject<UTagGroup>();
		FromContainer(gl, groupList);
		tagGroup->GroupList.Add(gl);
	}
}

FORCEINLINE void FromContainer(UInventoryTags* tags, const FInventoryTags& invTags) {
	tags->CurrentActiveIndex = invTags.CurrentActiveIndex;

	for (auto invTagGroup : invTags.InventoryGroupList)
	{
		auto igl = NewObject<UInventoryTagGroup>();
		FromContainer(igl, invTagGroup);
		tags->InventoryGroupList.Add(igl);
	}
}

//FORCEINLINE FInventoryTagGroup ToContainer() {
//	FInventoryTagGroup result;
//	result.Name = Name;
//	result.IsEnabled = IsEnabled;

//	for (auto tagGroup : GroupList)
//	{
//		if (tagGroup && tagGroup->IsValidLowLevel())
//			result.GroupList.Add(tagGroup->ToContainer());
//	}

//	return result;
//}


/*FORCEINLINE FTagGroup ToContainer() {
FTagGroup result;
result.GroupName = GroupName;
result.Tags = TArray<FString>(Tags);
return result;
}*/

