#pragma once

#include "ElectricityComponentDefinition.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FElectricityComponentDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityDefinition")
		float TotalObjectEnergy;

	FElectricityComponentDefinition()
	{
	};
};
