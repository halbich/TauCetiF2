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

	// TODO
	/*auto res = UBuildableBlockInfo::GetDefaultBuildableForID(id);

	res->Scale = dimensions;

	for (auto i = 0; i < flagNames.Num(); ++i)
	{
	res->AdditionalFlags.Add(flagNames[i], flagValues[i]);
	}

	return res;*/
	return true;
}
