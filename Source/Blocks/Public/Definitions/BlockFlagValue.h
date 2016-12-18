

#pragma once

#include "BlockFlagValue.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FBlockFlagValue
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block Flags")
		int32 Value;
	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block Flags")
		FText DisplayText;

	FBlockFlagValue()
	{

	};


};
