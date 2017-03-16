#pragma once

#include "OxygenComponentDefinition.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FOxygenComponentDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | OxygenDefinition")
		float TotalObjectVolume;

	FOxygenComponentDefinition()
	{
	};
};
