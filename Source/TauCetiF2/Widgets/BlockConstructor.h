

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
		bool AddItemToInventory(int32 id, FString name, FVector dimensions, TArray<FString> flagNames, TArray<int32> flagValues, TArray<FString> tags, UPARAM(ref)TArray<FString>& validationErrors);
	
private:
	//TODO localizace
	UBuildableBlockInfo* validate(int32 id, FVector dimensions, TArray<FString> flagNames, TArray<int32> flagValues, TArray<FString>& validationErrors )
	{
		auto definition = FBlockDefinitionHolder::Instance().GetDefinition(id);

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

		auto res = UBuildableBlockInfo::GetDefaultBuildableForID(id);

		res->Scale = dimensions;

		for (auto i = 0; i < flagNames.Num(); ++i)
		{
			res->AdditionalFlags.Add(flagNames[i], flagValues[i]);
		}

		return res;

	}
};
