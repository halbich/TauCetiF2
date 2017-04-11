#pragma once

#include "ElectricityBindableAreas.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityDefinition")
		FElectricityBindableAreas BindableAreas;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityDefinition")
		bool IsProducer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityDefinition")
		bool IsConsument;

	FElectricityComponentDefinition()
	{
	};
};
