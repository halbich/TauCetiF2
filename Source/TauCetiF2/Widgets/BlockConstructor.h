#pragma once

#include "Widgets/TerminalBaseWidget.h"
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

	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockConstructorSelector)
		UBlockHolderComponent* blockHolder;

	UFUNCTION(BlueprintCallable, Category = BlockConstructorSelector)
		bool AddItemToInventory(int32 id, FString name, FVector dimensions, TArray<FString> flagNames, TArray<int32> flagValues, TArray<FString> tags, UPARAM(ref)TArray<FString>& validationErrors);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BlockConstructorSelector)
		TArray<int32> GetAllAviableBlocks();

private:
	//TODO localizace
	UBuildableBlockInfo* validate(int32 id, FVector dimensions, TArray<FString> flagNames, TArray<int32> flagValues, TArray<FString>& validationErrors)
	{
		auto definition = blockHolder->GetDefinitionFor(id);

		if (!definition)
		{
			validationErrors.Add(TEXT("vadná definice"));
			return nullptr;
		}

		if (!definition->IsInLimits(dimensions))
		{
			validationErrors.Add(TEXT("Objekt není v dimenzi"));
			return nullptr;
		}

		if (!definition->ValidateFlags(flagNames, flagValues, validationErrors))
			return nullptr;

		// TODO
		/*auto res = UBuildableBlockInfo::GetDefaultBuildableForID(id);

		res->Scale = dimensions;

		for (auto i = 0; i < flagNames.Num(); ++i)
		{
			res->AdditionalFlags.Add(flagNames[i], flagValues[i]);
		}

		return res;*/
		return nullptr;
	}
};
