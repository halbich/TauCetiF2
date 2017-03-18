

#pragma once

#include "FWeatherState.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FWeatherState& state)
{
	Ar << state.CurrentDefinitionID;
	Ar << state.IsInWeatherChange;
	Ar << state.CurrentWaitingTime;
	Ar << state.TargetWaitingTime;

	Ar << state.CurrentWeatherIntensity;
	Ar << state.TargetWeatherIntensity;
	Ar << state.CurrentCloudOpacity;
	Ar << state.TargetCloudOpacity;

	return Ar;
}
