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
}