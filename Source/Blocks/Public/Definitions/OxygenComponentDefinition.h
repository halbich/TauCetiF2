#pragma once

#include "OxygenComponentDefinition.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct BLOCKS_API FOxygenComponentDefinition
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | OxygenDefinition")
		float TotalObjectVolume;

	FOxygenComponentDefinition()
	{
	};
};
