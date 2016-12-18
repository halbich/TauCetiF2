

#include "TauCetiF2.h"
#include "BlockConstructor.h"


UBlockConstructor::UBlockConstructor() {

}


bool UBlockConstructor::AddItemToInventory(int32 id, FString name, FVector dimensions, TArray<FString> flagNames, TArray<int32> flagValues, TArray<FString> tags, UPARAM(ref)TArray<FString>& validationErrors)
{
	validationErrors.Empty();

	auto bi = validate(id, dimensions, flagNames, flagValues, validationErrors);
	if (!bi)
		return false;

	auto pc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!pc || !pc->IsValidLowLevel())
		return false;

	auto tcf2Char = Cast<ATauCetiF2Character>(pc);
	if (!tcf2Char || !tcf2Char->IsValidLowLevel())
		return false;

	bi->Name = name;
	bi->Tags = tags;

	tcf2Char->GetInventory()->AddItem(bi);

	return true;
}


TArray<int32> UBlockConstructor::GetAllAviableBlocks()
{

	if (blockHolder && blockHolder->IsValidLowLevel())
	{
		return blockHolder->GetAviableItems();
	}


	TArray<int32> result;

	blockHolder = UBlockHolderComponent::GetInstance();

	if (!blockHolder)
		return result;

	return blockHolder->GetAviableItems();
}

