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
		validationErrors.Add(NSLOCTEXT("TCF2LocSpace", "LC.BuildableBlockInfo.No_Definition", "Blok má neplatnou definici."));
		return false;
	}

	if (!definition->IsInLimits(Scale))
	{
		validationErrors.Add(NSLOCTEXT("TCF2LocSpace", "LC.BuildableBlockInfo.Invalid_Dimensions", "Blok není v rozmezí platné velikosti."));
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
