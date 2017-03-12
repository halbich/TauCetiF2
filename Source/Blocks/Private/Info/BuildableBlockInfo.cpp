#include "Blocks.h"
#include "BuildableBlockInfo.h"

UBuildableBlockInfo::UBuildableBlockInfo() : UBlockBaseInfo(), Action(EBuildableObjectAction::None), AllowOutlineOnSelected(true)
{
}

void UBuildableBlockInfo::DefinitionSet()
{
	switch (ID)
	{
	case EmptyHandID: {
		Action = EBuildableObjectAction::None;
		break;
	}
	case DeleteID: {
		StencilOverride = STENCIL_DELETE_OUTLINE;
		Action = EBuildableObjectAction::DeleteObject;
		break;
	}
	default:
		Action = EBuildableObjectAction::ConstructObject;
		break;
	}

}

bool UBuildableBlockInfo::ValidateObject(TArray<FText>& validationErrors)
{
	auto definition = BlockDefinition;

	if (!definition || !definition->IsValidLowLevel())
		definition = UBlockHolderComponent::GetInstance()->GetDefinitionFor(ID);

	if (!definition)
	{
		validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.BuildableBlockInfo.No_Definition", "Blok s ID {0} má neplatnou definici."), ID));
		return false;
	}

	if (!definition->IsInLimits(Scale))
	{
		validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.BuildableBlockInfo.Invalid_Dimensions", "Blok s ID {0} není v rozmezí platné velikosti. (Min: {1}, Max: {2}, Scale: {3})"), FText::AsNumber(ID), definition->MinBlockScale.ToText(), definition->MaxBlockScale.ToText(), (definition->HasCustomScaling ? definition->CustomBlockScale : Scale).ToText()));
		return false;
	}


	if (!definition->ValidateFlags(AdditionalFlags, validationErrors))
		return false;

	return true;
}


UBuildableBlockInfo* UBuildableBlockInfo::GetBuildable(UBlockDefinition* def)
{
	auto res = NewObject<UBuildableBlockInfo>();
	res->ID = def->BlockID;
	res->Scale = def->GetObjectScale(def->MinBlockScale);

	res->BlockDefinition = def;

	res->DefinitionSet();

	return res;

}

UBuildableBlockInfo* UBuildableBlockInfo::GetCopy()
{
	auto res = NewObject<UBuildableBlockInfo>();
	res->ID = ID;
	res->Scale = Scale;
	res->Name = Name;
	res->AdditionalFlags.Append(AdditionalFlags);
	res->Tags.Append(Tags);
	res->Action = Action;
	res->AllowOutlineOnSelected = AllowOutlineOnSelected;
	res->StencilOverride = StencilOverride;
	res->BlockDefinition = BlockDefinition;
	res->IsInventoryItemInfo = IsInventoryItemInfo;
	res->DisplayValue = DisplayValue;

	return res;

}