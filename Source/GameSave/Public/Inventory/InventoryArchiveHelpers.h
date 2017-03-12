

#pragma once

#include "FTagGroup.h"
#include "FInventoryTagGroup.h"
#include "FInventoryTags.h"

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
	Ar << tagGroup.GroupType;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FInventoryTags& invTags)
{
	Ar << invTags.CurrentActiveIndex;
	Ar << invTags.InventoryGroupList;
	return Ar;
}
