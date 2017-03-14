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
		DisplayValue = CustomFormat(OxygenInfo->CurrentFillingValue);
		DisplayValueName = NSLOCTEXT("TCF2LocSpace", "LC.UInventoryBuildableBlockInfo.OxygenDisplayName", "Obsah kyslíku:");
	}

	if (ElectricityInfo)
	{
		DisplayValue = CustomFormat(ElectricityInfo->CurrentObjectEnergy);
		DisplayValueName = NSLOCTEXT("TCF2LocSpace", "LC.UInventoryBuildableBlockInfo.ElectricityDisplayName", "Obsah energie:");
	}
}

void UInventoryBuildableBlockInfo::DefinitionSet()
{
	Super::DefinitionSet();
	BuildingEnergyRequired = 0.0f;
}

UInventoryBuildableBlockInfo* UInventoryBuildableBlockInfo::GetInventoryBuildable(UBlockBaseInfo* info, UBlockDefinition* def)
{
	auto res = NewObject<UInventoryBuildableBlockInfo>();

	res->ID = info->ID;
	res->Scale = info->Scale;
	res->Name = info->Name;
	res->AdditionalFlags = info->AdditionalFlags;
	res->OxygenInfo = info->OxygenInfo;
	res->ElectricityInfo = info->ElectricityInfo;

	res->BlockDefinition = def;

	res->DefinitionSet();
	res->UpdateDisplayValue();

	return res;
}