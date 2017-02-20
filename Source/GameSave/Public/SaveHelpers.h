

#pragma once

#include "BlockSaveInfo.h"
#include "Blocks/Public/Info/BlockBaseInfo.h"
#include "Blocks/Public/Info/BlockInfo.h"
#include "Blocks/Public/Info/BuildableBlockInfo.h"
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
	
};



/*FORCEINLINE FBlockInfo ToContainer() {
FBlockInfo result = ToBaseContainer();
result.Location = Location;
result.Rotation = Rotation;
return result;
}

FORCEINLINE void FromContainer(const FBlockInfo& block) {

FromBaseContainer(block);
Location = block.Location;
Rotation = block.Rotation;
}*/

/*FORCEINLINE FBlockBaseInfo ToBaseContainer() {
FBlockBaseInfo result;
result.ID = ID;
result.Scale = Scale;
result.Name = Name;
result.AdditionalFlags = AdditionalFlags;
return result;
}


FORCEINLINE void FromBaseContainer(const FBlockBaseInfo& block) {
ID = block.ID;
Scale = block.Scale;
Name = block.Name;
AdditionalFlags = block.AdditionalFlags;
}*/


//FORCEINLINE FInventoryBuildableBlockInfo ToContainer() {
//	FInventoryBuildableBlockInfo result; // = ToBaseContainer();
//	//TODO
//	result.Tags = Tags;
//	return result;
//}

//FORCEINLINE void FromContainer(FInventoryBuildableBlockInfo& info) {
//	//FromBaseContainer(info);
//	Tags = info.Tags;
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

FORCEINLINE void FromContainer(const FInventoryTags& invTags) {


CurrentActiveIndex = invTags.CurrentActiveIndex;

for (auto invTagGroup : invTags.InventoryGroupList)
{
auto igl = NewObject<UInventoryTagGroup>();
igl->FromContainer(invTagGroup);
InventoryGroupList.Add(igl);
}

}*/




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

//FORCEINLINE void FromContainer(const FInventoryTagGroup& invTagGroup) {


//	Name = invTagGroup.Name;
//	IsEnabled = invTagGroup.IsEnabled;

//	for (auto groupList : invTagGroup.GroupList)
//	{
//		auto gl = NewObject<UTagGroup>(this);
//		gl->FromContainer(groupList);
//		GroupList.Add(gl);
//	}

//}

/*FORCEINLINE FTagGroup ToContainer() {
FTagGroup result;
result.GroupName = GroupName;
result.Tags = TArray<FString>(Tags);
return result;
}*/

/*FORCEINLINE void FromContainer(const FTagGroup& group) {
GroupName = group.GroupName;
Tags = TArray<FString>(group.Tags);
}*/
