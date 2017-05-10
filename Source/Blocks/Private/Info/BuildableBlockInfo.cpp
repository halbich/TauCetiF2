#include "Blocks.h"
#include "BuildableBlockInfo.h"

UBuildableBlockInfo::UBuildableBlockInfo() : UBlockBaseInfo(), Action(EBuildableObjectAction::None), AllowOutlineOnSelected(true)
{
}

void UBuildableBlockInfo::DefinitionSet()
{
	switch (ID)
	{
		/*case EmptyHandID: {
			Action = EBuildableObjectAction::None;
			AllowOutlineOnSelected = false;
			break;
		}*/
	case DeleteID: {
		StencilOverride = STENCIL_DELETE_OUTLINE;
		Action = EBuildableObjectAction::DeleteObject;
		ImplicitTags.Add(BlockDefinition->BlockDisplayName.ToString());
		break;
	}
	default:
		Action = EBuildableObjectAction::ConstructObject;
		auto baseEnergyReq = BlockDefinition->EnergyReqiredToBuild * buildingCoeficient();
		auto dimensions = BlockDefinition->HasCustomScaling ? FVector(1, 1, 1) : Scale;
		BuildingEnergyRequired = baseEnergyReq * dimensions.X * dimensions.Y * dimensions.Z;

		ImplicitTags.Add(BlockDefinition->BlockDisplayName.ToString());
		ImplicitTags.Add(TEXT("X-") + FString::FormatAsNumber(FMath::Floor(Scale.X)));
		ImplicitTags.Add(TEXT("Y-") + FString::FormatAsNumber(FMath::Floor(Scale.Y)));
		ImplicitTags.Add(TEXT("Z-") + FString::FormatAsNumber(FMath::Floor(Scale.Z)));


		break;
	}
}

bool UBuildableBlockInfo::ValidateObject(TArray<FText>& validationErrors, UBlockHolder* holder)
{
	auto definition = BlockDefinition;

	if (!definition || !definition->IsValidLowLevel())
		definition = holder->GetDefinitionFor(ID);

	if (!definition)
	{
		validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.BuildableBlockInfo.No_Definition", "Blok s ID {0} má neplatnou definici."), ID));
		return false;
	}

	if (!definition->IsInLimits(Scale))
	{
		validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.BuildableBlockInfo.Invalid_Dimensions", "Blok inventáře s ID {0} není v rozmezí platné velikosti. (Min: {1}, Max: {2}, Velikost: {3})"), FText::AsNumber(ID), (definition->HasCustomScaling ? definition->CustomBlockScale : definition->MinBlockScale).ToText(), (definition->HasCustomScaling ? definition->CustomBlockScale : definition->MaxBlockScale).ToText(), Scale.ToText()));
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
	res->OxygenInfo = OxygenInfo;
	res->ElectricityInfo = ElectricityInfo;

	res->Tags.Append(Tags);
	res->Action = Action;
	res->AllowOutlineOnSelected = AllowOutlineOnSelected;
	res->StencilOverride = StencilOverride;
	res->BlockDefinition = BlockDefinition;
	res->DisplayValue = DisplayValue;

	return res;
}