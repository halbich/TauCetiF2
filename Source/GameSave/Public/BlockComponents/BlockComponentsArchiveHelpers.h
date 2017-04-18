#pragma once

#include "OxygenComponentInfo.h"
#include "ElectricityComponentInfo.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FOxygenComponentInfo& componentInfo)
{
	Ar << componentInfo.CurrentObjectOxygen;
	Ar << componentInfo.OxygenConsumptionPercent;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FElectricityComponentInfo& componentInfo)
{
	Ar << componentInfo.CurrentObjectEnergy;
	Ar << componentInfo.PowerConsumptionPercent;
	return Ar;
}