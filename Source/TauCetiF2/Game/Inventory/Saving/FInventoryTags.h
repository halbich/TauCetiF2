

#pragma once

#include "Game/Inventory/Saving/FInventoryTagGroup.h"

/**
 *
 */
class TAUCETIF2_API FInventoryTags
{
public:
	FInventoryTags();

	int32 CurrentActiveIndex;
	TArray<FInventoryTagGroup> InventoryGroupList;

};
