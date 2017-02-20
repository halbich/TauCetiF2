

#include "Inventory.h"
#include "InventoryTags.h"


const int32 UInventoryTags::DefaultCount = 10;

UInventoryTags* UInventoryTags::GetDefault()
{
	auto res = NewObject<UInventoryTags>();
	for (int32 i = 0; i < DefaultCount; i++)
	{
		auto itm = NewObject<UInventoryTagGroup>();
		if (i == 0)
		{
			itm->IsEnabled = true;
			res->CurrentActiveIndex = 0;
		}

		itm->Name = FString::Printf(TEXT("Group %d"), i + 1);
		res->InventoryGroupList.Add(itm);
	}

	return res;
}

FDelegateHandle UInventoryTags::AddEventListener(FSelectionChanged& UseDelegate)
{
	return OnSelectionChanged.Add(UseDelegate);
}

void UInventoryTags::RemoveEventListener(FDelegateHandle DelegateHandle)
{
	OnSelectionChanged.Remove(DelegateHandle);
}