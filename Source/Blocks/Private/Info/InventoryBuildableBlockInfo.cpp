#include "Blocks.h"
#include "InventoryBuildableBlockInfo.h"

UInventoryBuildableBlockInfo::UInventoryBuildableBlockInfo()
	:UBuildableBlockInfo()
{

}

void UInventoryBuildableBlockInfo::UpdateDisplayValue()
{
	if (OxygenInfo)
	{
		DisplayValue = FText::FromString(FString::SanitizeFloat(OxygenInfo->CurrentFillingValue));
		DisplayValueName = NSLOCTEXT("TCF2LocSpace", "LC.UInventoryBuildableBlockInfo.OxygenDisplayName", "Obsah kyslíku:");
	}

	if (ElectricityInfo)
	{
		DisplayValue = FText::FromString(FString::SanitizeFloat(ElectricityInfo->CurrentObjectEnergy));
		DisplayValueName = NSLOCTEXT("TCF2LocSpace", "LC.UInventoryBuildableBlockInfo.ElectricityDisplayName", "Obsah energie:");
	}
}