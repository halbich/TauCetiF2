#pragma once

#include "Info/BuildableBlockInfo.h"
#include "InventoryBuildableBlockInfo.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API UInventoryBuildableBlockInfo : public UBuildableBlockInfo
{
	GENERATED_BODY()

public:

	UInventoryBuildableBlockInfo();

	virtual void UpdateDisplayValue();

	virtual void DefinitionSet() override;

	static UInventoryBuildableBlockInfo* GetInventoryBuildable(UBlockBaseInfo* info, UBlockDefinition* def);

private:
	FORCEINLINE FText CustomFormat(float Value)
	{
		FNumberFormattingOptions NumberFormatOptions;
		NumberFormatOptions.UseGrouping = true;
		NumberFormatOptions.RoundingMode = ERoundingMode::HalfToEven;
		NumberFormatOptions.MinimumIntegralDigits = 1;
		NumberFormatOptions.MaximumIntegralDigits = 324;
		NumberFormatOptions.MinimumFractionalDigits = 2;
		NumberFormatOptions.MaximumFractionalDigits = 2;

		return FText::AsNumber(Value, &NumberFormatOptions);
	}
};
