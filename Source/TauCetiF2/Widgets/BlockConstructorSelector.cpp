

#include "TauCetiF2.h"
#include "BlockConstructorSelector.h"



UBlockConstructorSelector::UBlockConstructorSelector()
{
	
}


void UBlockConstructorSelector::SynchronizeProperties()
{
	if (BlockName != EBlockName::Empty)
	{
		Definition = FBlockDefinitionHolder::Instance().GetDefinition((int32)BlockName);

		UBuildableBlockInfo* info = NewObject<UBuildableBlockInfo>();
		info->ID = Definition->ID;
		BlockTexture = UHelpers::GetTexture2DForBlock(info);
	}

	Super::SynchronizeProperties();
}