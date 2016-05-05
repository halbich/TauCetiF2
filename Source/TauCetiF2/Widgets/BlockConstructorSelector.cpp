

#include "TauCetiF2.h"
#include "BlockConstructorSelector.h"



UBlockConstructorSelector::UBlockConstructorSelector()
{

}


void UBlockConstructorSelector::SynchronizeProperties()
{
	Definition = FBlockDefinitionHolder::Instance().GetDefinition(BlockID, false);
	if (!Definition)
	{
		Super::SynchronizeProperties();
		return;
	}

	UBuildableBlockInfo* info = NewObject<UBuildableBlockInfo>();
	info->ID = Definition->ID;
	BlockTexture = UHelpers::GetTexture2DForBlock(info);

	Super::SynchronizeProperties();
}