#include "Blocks.h"
#include "BlockWithElectricityInfo.h"

float UBlockWithElectricityInfo::GetPowerOutput() const
{
	ensure(PoweredBlockInfo);

	return PoweredBlockInfo->GetCurrentPowerConsumptionPercent(GetRemainingPercentage());
}