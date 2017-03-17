#include "Inventory.h"
#include "InventoryTags.h"




FDelegateHandle UInventoryTags::AddEventListener(FSelectionChanged& UseDelegate)
{
	return OnSelectionChanged.Add(UseDelegate);
}

void UInventoryTags::RemoveEventListener(FDelegateHandle DelegateHandle)
{
	OnSelectionChanged.Remove(DelegateHandle);
}