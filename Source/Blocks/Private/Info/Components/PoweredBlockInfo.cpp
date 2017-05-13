

#include "Blocks.h"
#include "PoweredBlockInfo.h"

UPoweredBlockInfo::UPoweredBlockInfo() {

	IsOn = true;
	AutoregulatePower = true;
	PowerConsumptionPercent = 1;
}


float UPoweredBlockInfo::GetCurrentPowerConsumptionPercent(const float remainingEnergyPercent)
{
	if (!IsOn)
		return 0.0f;

	if (!AutoregulatePower)
		return PowerConsumptionPercent;

	if (remainingEnergyPercent >= 50.0f)
		return 1.0f;

	if (remainingEnergyPercent >= 25.0f)
		return .5f;

	return 0.1f;
}
