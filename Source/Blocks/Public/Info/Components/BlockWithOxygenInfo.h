#pragma once

#include "UObject/NoExportTypes.h"
#include "BlockWithOxygenInfo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class BLOCKS_API UBlockWithOxygenInfo : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockWithOxygenInfo")
		float CurrentObjectOxygen;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockWithOxygenInfo")
		float CurrentObjectMaximumOxygen;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockWithElectricityInfo")
		float OxygenConsumptionPercent;

	FORCEINLINE const float GetRemainingPercentageUnit() const
	{
		return FMath::IsNearlyZero(CurrentObjectMaximumOxygen)
			? 0.0f
			: (CurrentObjectOxygen / CurrentObjectMaximumOxygen);
	}

	FORCEINLINE const float GetRemainingPercentage() const
	{
		return GetRemainingPercentageUnit() * 100.0f;
	}
};
