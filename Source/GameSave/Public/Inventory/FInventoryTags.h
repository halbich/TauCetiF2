#pragma once

#include "FInventoryTagGroup.h"

/**
 *
 */
struct GAMESAVE_API FInventoryTags
{
	FInventoryTags();

	int32 CurrentActiveIndex;
	TArray<FInventoryTagGroup> InventoryGroupList;

	void Empty();
};
