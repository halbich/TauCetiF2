#pragma once

/**
* Saving structure for Electricity component
*/
struct FElectricityComponentInfo {
	float CurrentObjectEnergy;
	/*
	Default constructor
	*/
	FElectricityComponentInfo()
	{
		CurrentObjectEnergy = 0;
	};
};
