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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityDefinition")
		float MaxConsumedEnergyPerGameSecond;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityDefinition")
		bool IsControlBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityDefinition", meta = (EditCondition = IsControlBlock))
		bool IsController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityDefinition", meta = (EditCondition = IsControlBlock))
		bool IsControllable;

	FElectricityComponentDefinition()
	{
	};
};
