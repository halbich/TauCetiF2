

#pragma once

#include "OxygenComponentInfo.h"
#include "ElectricityComponentInfo.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FOxygenComponentInfo& componentInfo)
{
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FElectricityComponentInfo& componentInfo)
{
	return Ar;
}