#pragma once

/**
* Saving structure for Electricity component
*/
struct GAMESAVE_API FElectricityComponentInfo {
	float CurrentObjectEnergy;
	/*
	Default constructor
	*/
	FElectricityComponentInfo() :CurrentObjectEnergy(0)
	{};
};
