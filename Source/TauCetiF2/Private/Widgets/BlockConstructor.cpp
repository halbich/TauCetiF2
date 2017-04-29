#include "TauCetiF2.h"
#include "BlockConstructor.h"

UBlockConstructor::UBlockConstructor() {
}

bool UBlockConstructor::AddItemToInventory(UBuildableBlockInfo* buildable, TArray<FText>& validationErrors)
{
	validationErrors.Empty();
	if (!buildable || !buildable->IsValidLowLevel())
	{
		validationErrors.Add(NSLOCTEXT("TCF2LocSpace", "LC.BlockConstructor.InvalidBuildbale", "Vstup není korektní."));
		return false;
	}

	auto bi = buildable->ValidateObject(validationErrors, blockHolder);
	if (!bi)
		return false;

	auto pc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!pc || !pc->IsValidLowLevel())
	{
		validationErrors.Add(NSLOCTEXT("TCF2LocSpace", "LC.BlockConstructor.InvalidPC", "Neplatný PlayerCharacter."));
		return false;
	}
	auto tcf2Char = Cast<ATauCetiF2Character>(pc);
	if (!tcf2Char || !tcf2Char->IsValidLowLevel())
	{
		validationErrors.Add(NSLOCTEXT("TCF2LocSpace", "LC.BlockConstructor.InvalidTcf2PC", "Neplatný ATauCetiF2Character."));
		return false;
	}

	auto bc = buildable->GetCopy();
	AddImplicitTags(bc);
	tcf2Char->GetInventory()->AddItem(bc);

	return true;
}

TArray<UBuildableBlockInfo*> UBlockConstructor::GetAllBuildableBlocks()
{
	TArray<UBuildableBlockInfo*> result;

	if (!ensureHolder() || !RelatedToPatternGroup || !RelatedToPatternGroup->IsValidLowLevel())
		return result;

	auto v = RelatedToPatternGroup->IsValidCreator;

	for (auto buildable : blockHolder->GetAviableItems())
	{
		auto def = blockHolder->GetDefinitionFor(buildable);

		if (!def->IsSystemAction && def->IsBuildable)
		{
			auto b = UBuildableBlockInfo::GetBuildable(def);
			auto max = def->HasCustomScaling ? def->CustomBlockScale : def->MinBlockScale;
			auto limit = FMath::Max(ScaleLimit.X, ScaleLimit.Y);
			b->BlockConstructorDisabled = !v || max.X > limit || max.Y > limit || max.Z > limit;
			result.Add(b);
		}
	}

	return result;
}

void UBlockConstructor::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	ensure(RelatedToPatternGroup);

	auto wb = RelatedToPatternGroup->WatchingBox;

	ensure(wb);

	ScaleLimit = ((wb->Max - wb->Min) / GameDefinitions::CubeMinSize).GridSnap(1);
	ScaleLimit.Z = FMath::Max(ScaleLimit.X, ScaleLimit.Y);
}

FText UBlockConstructor::GetDisplayTextExtended_Implementation()
{
	auto val = NSLOCTEXT("TCF2LocSpace", "LC.BlockConstructor.CtorValid", "Online");
	auto inval = NSLOCTEXT("TCF2LocSpace", "LC.BlockConstructor.CtorInvalid", "Offline");

	FNumberFormattingOptions op;
	op.SetMinimumFractionalDigits(0);
	op.SetMaximumFractionalDigits(0);

	FFormatOrderedArguments Arguments;
	Arguments.Add(BaseControlDisplayName);
	Arguments.Add(FText::AsNumber(FMath::Max(ScaleLimit.X, ScaleLimit.Y), &op));
	Arguments.Add(RelatedToPatternGroup->IsValidCreator ? val : inval);
	return FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.BlockConstructor.ExtendedText", "{0}    |    Velikost: {1}    |    Stav: {2}"), Arguments);
}