

#pragma once

#include "Widgets/TerminalBaseWidget.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "Game/TauCetiF2Character.h"
#include "BlockConstructor.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UBlockConstructor : public UTerminalBaseWidget
{
	GENERATED_BODY()
	
public:

	UBlockConstructor();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BlockConstructorSelector)
		TArray<int32> AllAviableBlocks;
	
	UFUNCTION(BlueprintCallable, Category = BlockConstructorSelector)
		bool AddItemToInventory(int32 id, FString name, FVector dimensions, TArray<FString> flagNames, TArray<int32> flagValues, TArray<FString> tags);
	
private:

	UBuildableBlockInfo* validate(int32 id, FVector dimensions, TArray<FString> flagNames, TArray<int32> flagValues)
	{
		auto definition = FBlockDefinitionHolder::Instance().GetDefinition(id);

		if (!definition)
			return nullptr;

		if (!definition->IsInLimits(dimensions))
			return nullptr;

		if (!definition->ValidateFlags(flagNames, flagValues))
			return nullptr;

		auto res = UBuildableBlockInfo::GetDefaultBuildableForID(id);
		//TODO 
		return res;

	}
};
