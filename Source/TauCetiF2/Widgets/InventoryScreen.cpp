

#include "TauCetiF2.h"
#include "InventoryScreen.h"




bool UInventoryScreen::CanUncheck()
{
	check(InventoryComponent && InventoryComponent->IsValidLowLevel());
	return InventoryComponent->InventoryTags->GetEnabledGroupsCount() > 1;
}

void UInventoryScreen::CheckCurrentSelection()
{
	check(InventoryComponent && InventoryComponent->IsValidLowLevel());
	InventoryComponent->InventoryTags->CheckCurrentSelection();
}