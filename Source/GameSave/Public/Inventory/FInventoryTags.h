

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

};
