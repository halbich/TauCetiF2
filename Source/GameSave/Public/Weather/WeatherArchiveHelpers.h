

#pragma once

#include "FWeatherState.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FWeatherState& state)
{
	Ar << state.CurrentDefinitionID;
	Ar << state.CurrentWorldTime;
	Ar << state.CurrentWaitingTime;
	Ar << state.IsInWeatherChange;

	return Ar;
}
