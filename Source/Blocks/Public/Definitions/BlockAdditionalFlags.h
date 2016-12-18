

#pragma once
#include "BlockFlagValue.h"
#include "BlockAdditionalFlags.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FBlockAdditionalFlags
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block Flags")
		FString TagID;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block Flags")
		FText DisplayText;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block Flags", meta = (ShowOnlyInnerProperties))
		TArray<FBlockFlagValue> PossibleValues;

	FBlockAdditionalFlags()
	{

	};


};
