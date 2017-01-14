

#include "TauCetiF2.h"
#include "BlockBaseInfo.h"




TArray<UInventoryFlagItem*> UBlockBaseInfo::GetBlockFlags()
{
	TArray<UInventoryFlagItem*> result;

	if (ID <= 0)
		return result; // system actions

	auto def = UBlockHolderComponent::GetInstance()->GetDefinitionFor(ID);
	ensure(def);

	for (auto fl : def->AdditionalFlags)
	{
		auto invItem = NewObject<UInventoryFlagItem>(this);
		invItem->TagName = fl.TagID;
		invItem->DisplayText = fl.DisplayText;

		for (auto it : fl.PossibleValues)
		{
			auto cmbIt = NewObject<UCmbItem>(this);
			cmbIt->Value = it.Value;
			cmbIt->Text = it.DisplayText;
			invItem->AviableValues.Add(cmbIt);
		}

		if (AdditionalFlags.Contains(fl.TagID))
		{
			invItem->TagValue = AdditionalFlags[fl.TagID];
			invItem->TagReadOnly = true;
			result.Add(invItem);
		}


	}


	return result;
}