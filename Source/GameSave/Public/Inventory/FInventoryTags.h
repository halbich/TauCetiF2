#pragma once

#include "Inventory/FInventoryTagGroup.h"

/**
 *
 */
class GAMESAVE_API FInventoryTags
{
public:
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
