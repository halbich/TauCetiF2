#include "Blocks.h"
#include "InventoryBuildableBlockInfo.h"

UInventoryBuildableBlockInfo::UInventoryBuildableBlockInfo()
	:UBuildableBlockInfo()
{

}

void UInventoryBuildableBlockInfo::UpdateDisplayValue()
{
	if (OxygenInfo)
		DisplayValue = FText::FromString(FString::SanitizeFloat(OxygenInfo->CurrentFillingValue));

	if (ElectricityInfo)
		DisplayValue = FText::FromString(FString::SanitizeFloat(ElectricityInfo->CurrentObjectEnergy));
}