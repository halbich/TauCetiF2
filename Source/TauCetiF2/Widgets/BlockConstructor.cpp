

#include "TauCetiF2.h"
#include "BlockConstructor.h"


UBlockConstructor::UBlockConstructor() {

	FBlockDefinitionHolder::Instance().GetAllDefinitions(AllAviableBlocks);

}


bool UBlockConstructor::AddItemToInventory(int32 id, FString name, FVector dimensions, TArray<FString> flagNames, TArray<int32> flagValues, TArray<FString> tags)
{

	auto bi = validate(id, dimensions, flagNames, flagValues);
	if (!bi)
		return false;

	auto pc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!pc || !pc->IsValidLowLevel())
		return false;

	auto tcf2Char = Cast<ATauCetiF2Character>(pc);
	if (!tcf2Char || !tcf2Char->IsValidLowLevel())
		return false;

	bi->Name = name;

	tcf2Char->GetInventory()->AddItem(bi);

	return false;
}



