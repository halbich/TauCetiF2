#pragma once

#include "PoweredBlockInfo.h"

/**
* Saving structure for Electricity component
*/
struct FElectricityComponentInfo {
	float CurrentObjectEnergy;
	bool HasPoweredBlockInfo;

	FPoweredBlockInfo PoweredBlockInfo;
	/*
	Default constructor
	*/
	FElectricityComponentInfo() : PoweredBlockInfo(), CurrentObjectEnergy(0), HasPoweredBlockInfo(false)
	{
	};
};
