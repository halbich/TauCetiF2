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

	auto bi = buildable->ValidateObject(validationErrors);
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
	ensureHolder();
	TArray<UBuildableBlockInfo*> result;
	if (!blockHolder)
		return result;

	for (auto buildable : blockHolder->GetAviableItems())
	{
		auto def = blockHolder->GetDefinitionFor(buildable);

		if (!def->IsSystemAction && def->IsBuildable)
			result.Add(UBuildableBlockInfo::GetBuildable(def));
	}

	return result;
}