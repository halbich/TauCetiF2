#pragma once

#include "OxygenComponentInfo.h"
#include "ElectricityComponentInfo.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FOxygenComponentInfo& componentInfo)
{
	Ar << componentInfo.CurrentObjectOxygen;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FElectricityComponentInfo& componentInfo)
{
	Ar << componentInfo.CurrentObjectEnergy;
	return Ar;
}