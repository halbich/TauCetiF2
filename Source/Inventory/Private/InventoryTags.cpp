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
			itm->IsGroupEnabled = true;
			res->CurrentActiveIndex = 0;
			itm->GroupType = EInventoryGroupType::Inventory;
		}
		if (i == 1)
		{
			itm->IsGroupEnabled = true;
			res->CurrentActiveIndex = 1;
		}

		itm->GroupName = FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.InventoryTags.Default_Group_Name", "Skupina {0}"), FText::AsNumber(i + 1)).ToString();
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