#pragma once

#include "FInventoryTagGroup.h"

/**
 *
 */
struct FInventoryTags
{

	FInventoryTags();

	int32 CurrentActiveIndex;
	TArray<FInventoryTagGroup> InventoryGroupList;

	void Empty();
};
