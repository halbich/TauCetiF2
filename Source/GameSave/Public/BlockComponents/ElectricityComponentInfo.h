#pragma once

/**
* Saving structure for Electricity component
*/
struct FElectricityComponentInfo {
	float CurrentObjectEnergy;
	float PowerConsumptionPercent;
	/*
	Default constructor
	*/
	FElectricityComponentInfo()
	{
		CurrentObjectEnergy = 0;
		PowerConsumptionPercent = 1;
	};
};
