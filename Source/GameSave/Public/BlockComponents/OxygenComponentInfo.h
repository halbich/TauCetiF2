#pragma once

/**
 * Saving structure for Oxygen component
 */
struct FOxygenComponentInfo {
	float CurrentObjectOxygen;
	float OxygenConsumptionPercent;
	/*
	Default constructor
	*/
	FOxygenComponentInfo()
	{
		CurrentObjectOxygen = 0;
		OxygenConsumptionPercent = 1;
	};
};