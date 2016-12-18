

#include "TauCetiF2.h"
#include "BlockConstructorSelector.h"



UBlockConstructorSelector::UBlockConstructorSelector()
{

}


bool UBlockConstructorSelector::IsDefinitionValid()
{
	if (BlockDefinition && BlockDefinition->IsValidLowLevel())
		return true;

	if (BlockHolder)
	{
		BlockDefinition = BlockHolder->GetDefinitionFor(BlockID);
	}

	return BlockDefinition && BlockDefinition->IsValidLowLevel();
}

void UBlockConstructorSelector::GetMinMax(FVector& minSize, FVector& maxSize)
{
	check(IsDefinitionValid());
	minSize = BlockDefinition->MinBlockScale;
	maxSize = BlockDefinition->MaxBlockScale;

	return;
}

void UBlockConstructorSelector::SynchronizeProperties()
{
	IsDefinitionValid();
	Super::SynchronizeProperties();
}

#pragma optimize("", off)
TArray<UInventoryFlagItem*> UBlockConstructorSelector::GetAdditionalParams()
{
	TArray<UInventoryFlagItem*> result;
	check(IsDefinitionValid());

	for (auto fl : BlockDefinition->AdditionalFlags)
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
	check(IsDefinitionValid());
	return result;
}

UTexture2D* UBlockConstructorSelector::GetBlockTexture()
{
	UTexture2D* result = NULL;
	check(IsDefinitionValid());


	return BlockDefinition->BlockImage;
}

#pragma optimize("", on)

