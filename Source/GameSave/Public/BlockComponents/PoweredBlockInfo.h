#pragma once

/**
* Saving structure for Powered block
*/
struct FPoweredBlockInfo {
	bool IsOn;
	bool AutoregulatePower;
	float PowerConsumptionPercent;

	/*
	Default constructor
	*/
	FPoweredBlockInfo() : IsOn(true), AutoregulatePower(true), PowerConsumptionPercent(1)
	{
	};
};
