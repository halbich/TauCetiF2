

#pragma once

#include "UObject/NoExportTypes.h"
#include "PoweredBlockInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BLOCKS_API UPoweredBlockInfo : public UObject
{
	GENERATED_BODY()
	
public:
	UPoweredBlockInfo();


	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | PoweredBlockInfo")
		bool IsOn;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | PoweredBlockInfo")
		bool AutoregulatePower;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | PoweredBlockInfo")
		float PowerConsumptionPercent;



	float GetCurrentPowerConsumptionPercent(const float remainingEnergyPercent);
	
};
