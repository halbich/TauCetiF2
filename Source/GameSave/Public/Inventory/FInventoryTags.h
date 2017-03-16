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

	FORCEINLINE void Empty() {
		CurrentActiveIndex = 0;

		for (auto igl : InventoryGroupList)
			igl.Empty();

		InventoryGroupList.Empty();
	}
};
