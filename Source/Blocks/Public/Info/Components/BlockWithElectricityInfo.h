﻿#pragma once

#include "UObject/NoExportTypes.h"
#include "PoweredBlockInfo.h"
#include "BlockWithElectricityInfo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class BLOCKS_API UBlockWithElectricityInfo : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockWithElectricityInfo")
		float CurrentObjectEnergy;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockWithElectricityInfo")
		float CurrentObjectMaximumEnergy;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockWithElectricityInfo")
		UPoweredBlockInfo* PoweredBlockInfo;

	FORCEINLINE const float GetRemainingPercentageUnit() const
	{
		return FMath::IsNearlyZero(CurrentObjectMaximumEnergy)
			? 0.0f
			: (CurrentObjectEnergy / CurrentObjectMaximumEnergy);
	}

	FORCEINLINE const float GetRemainingPercentage() const
	{
		return GetRemainingPercentageUnit() * 100.0f;
	}

	float GetPowerOutput() const;
};