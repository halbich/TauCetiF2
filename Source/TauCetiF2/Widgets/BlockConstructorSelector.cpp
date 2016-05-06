

#include "TauCetiF2.h"
#include "BlockConstructorSelector.h"



UBlockConstructorSelector::UBlockConstructorSelector()
{

}


bool UBlockConstructorSelector::IsValid()
{
	return Definition != nullptr;
}

void UBlockConstructorSelector::GetMinMax(UPARAM(ref)FVector& minSize, UPARAM(ref)FVector& maxSize)
{
	check(IsValid());
	minSize = Definition->MinBlockScale;
	maxSize = Definition->MaxBlockScale;

	return;
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