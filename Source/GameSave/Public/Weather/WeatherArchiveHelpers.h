#pragma once

#include "FWeatherState.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FWeatherState& state)
{
	Ar << state.CurrentDefinitionID;
	Ar << state.IsInWeatherChange;
	Ar << state.CurrentWaitingTime;
	Ar << state.TargetWaitingTime;

	Ar << state.BaseWeatherIntensity;
	Ar << state.CurrentWeatherIntensity;
	Ar << state.TargetWeatherIntensity;

	Ar << state.BaseCloudOpacity;
	Ar << state.CurrentCloudOpacity;
	Ar << state.TargetCloudOpacity;

	Ar << state.HitpointsCounter;
	Ar << state.PlayerHitpointCounter;
	Ar << state.EaseIn;
	Ar << state.EaseOut;
	Ar << state.StormState;

	return Ar;
}
