

#include "TauCetiF2.h"
#include "InventoryScreen.h"




bool UInventoryScreen::CanUncheck()
{
	return InventoryTags->GetEnabledGroupsCount() > 1;
}

void UInventoryScreen::CheckCurrentSelection()
{
	InventoryTags->CheckCurrentSelection();
}