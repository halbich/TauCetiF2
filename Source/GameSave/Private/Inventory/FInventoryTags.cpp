#include "GameSave.h"
#include "FInventoryTags.h"

FInventoryTags::FInventoryTags() : CurrentActiveIndex(0), InventoryGroupList()
{
}

void FInventoryTags::Empty() {
	CurrentActiveIndex = 0;

	for (auto igl : InventoryGroupList)
		igl.Empty();

	InventoryGroupList.Empty();
}