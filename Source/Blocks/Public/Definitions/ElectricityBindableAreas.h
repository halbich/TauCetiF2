#pragma once

#include "ElectricityBindableAreas.generated.h"

USTRUCT(BlueprintType)
struct FElectricityBindableArea
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityBindableAreasDefinitionPlanes")
		TArray<int32> Plane;

	FElectricityBindableArea()
	{
	};
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FElectricityBindableAreas
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityBindableAreasDefinition")
		TArray<FVector> UsedPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | ElectricityBindableAreasDefinitionPlanes")
		TArray<FElectricityBindableArea> Planes;

	FElectricityBindableAreas()
	{
	};
};
