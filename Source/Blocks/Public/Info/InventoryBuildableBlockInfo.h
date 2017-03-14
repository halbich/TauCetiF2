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
};
