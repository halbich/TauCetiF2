#pragma once

#include "OxygenComponentInfo.h"
#include "ElectricityComponentInfo.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FOxygenComponentInfo& componentInfo)
{
	Ar << componentInfo.CurrentObjectOxygen;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FPoweredBlockInfo& componentInfo)
{
	Ar << componentInfo.IsOn;
	Ar << componentInfo.AutoregulatePower;
	Ar << componentInfo.PowerConsumptionPercent;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FElectricityComponentInfo& componentInfo)
{
	Ar << componentInfo.CurrentObjectEnergy;
	Ar << componentInfo.HasPoweredBlockInfo;

	if (componentInfo.HasPoweredBlockInfo)
		Ar << componentInfo.PoweredBlockInfo;

	return Ar;
}