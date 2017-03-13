#pragma once

#include "ElectricityComponentDefinition.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct BLOCKS_API FElectricityComponentDefinition
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityDefinition")
		float TotalObjectEnergy;

	FElectricityComponentDefinition()
	{
	};
};
