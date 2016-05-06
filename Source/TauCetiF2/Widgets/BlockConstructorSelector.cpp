

#include "TauCetiF2.h"
#include "BlockConstructorSelector.h"



UBlockConstructorSelector::UBlockConstructorSelector()
{

}


bool UBlockConstructorSelector::IsValid()
{
	return Definition != nullptr;
}

void UBlockConstructorSelector::GetMinMax(FVector& minSize, FVector& maxSize)
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

#pragma optimize("", off)
TArray<UInventoryFlagItem*> UBlockConstructorSelector::GetAdditionalParams()
{
	TArray<UInventoryFlagItem*> result;
	check(IsValid());

	for (auto fl : Definition->AdditionalFlags)
	{
		auto invItem = NewObject<UInventoryFlagItem>();
		invItem->TagName = fl.TagID;
		invItem->DisplayText = fl.DisplayText;

		for (auto it : fl.PossibleValues)
		{
			auto cmbIt = NewObject<UCmbItem>();
			cmbIt->Value = it.Value;
			cmbIt->Text = it.DisplayText;
			invItem->AviableValues.Add(cmbIt);
		}
		result.Add(invItem);
	}


	return result;
}


TArray<FString> UBlockConstructorSelector::GetImplicitTags()
{
	TArray<FString> result;
	check(IsValid());
	return result;
}

#pragma optimize("", on)

