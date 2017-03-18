

#pragma once

#include "FWeatherState.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FWeatherState& state)
{
	Ar << state.CurrentDefinitionID;
	Ar << state.CurrentWaitingTime;
	Ar << state.TargetWaitingTime;
	Ar << state.IsInWeatherChange;

	return Ar;
}
