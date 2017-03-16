#include "Inventory.h"
#include "InventoryTags.h"


UInventoryTags* UInventoryTags::GetDefault()
{
	// TODO check aginst gamesave, ii could be removed from here and elt this method live only in gamesave

	auto res = NewObject<UInventoryTags>();
	for (int32 i = 0; i < GameDefinitions::DefaultInventoryTagGroupsCount; i++)
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