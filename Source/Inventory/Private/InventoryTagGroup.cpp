#include "Inventory.h"
#include "InventoryTagGroup.h"

UTagGroup* UInventoryTagGroup::AddTagGroup(UTagGroup* group)
{
	GroupList.Add(group);
	IsBuildableCacheValid = false;
	IsInventoryCacheValid = false;
	return group;
}

bool UInventoryTagGroup::RemoveTagGroup(UTagGroup* group)
{
	auto rem = GroupList.Remove(group);
	IsBuildableCacheValid = false;
	IsInventoryCacheValid = false;
	return rem != 0;
}

void UInventoryTagGroup::InvalidateGroup()
{
	IsBuildableCacheValid = false;
	IsInventoryCacheValid = false;
}