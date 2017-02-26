#pragma once

#include "BlockSaveInfo.h"
#include "Inventory/FTagGroup.h"
#include "Inventory/FInventoryTagGroup.h"
#include "Inventory/FInventoryTags.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockBaseInfo& block)
{
	Ar << block.ID;
	Ar << block.Scale;
	Ar << block.Name;
	Ar << block.AdditionalFlags;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockInfo& block)
{
	Ar << (FBlockBaseInfo&)block;
	Ar << block.Location;
	Ar << block.Rotation;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FInventoryBuildableBlockInfo& block)
{
	Ar << (FBlockBaseInfo&)block;
	Ar << block.Tags;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FTagGroup& group)
{
	Ar << group.GroupName;
	Ar << group.Tags;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FInventoryTagGroup& tagGroup)
{
	Ar << tagGroup.GroupName;
	Ar << tagGroup.IsGroupEnabled;
	Ar << tagGroup.GroupList;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FInventoryTags& invTags)
{
	Ar << invTags.CurrentActiveIndex;
	Ar << invTags.InventoryGroupList;
	return Ar;
}
