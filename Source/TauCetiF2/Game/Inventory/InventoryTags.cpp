

#include "TauCetiF2.h"
#include "InventoryTags.h"


const int32 UInventoryTags::DefaultCount = 10;

UInventoryTags* UInventoryTags::GetDefault()
{
	auto res = NewObject<UInventoryTags>();
	for (int32 i = 0; i < DefaultCount; i++)
	{
		auto itm = NewObject<UInventoryTagGroup>();
		itm->Index = i;
		if (i == 0)
			itm->IsEnabled = true;

		itm->Name = TEXT("Lorem ipsum dolor sit ame");
		res->InventoryGroupList.Add(itm);
	}

	return res;
}