

#pragma once

/**
 *
 */
struct GAMESAVE_API FWeatherState
{
public:
	FWeatherState();

	int32 CurrentDefinitionID;
	bool  IsInWeatherChange;
	float CurrentWaitingTime;
	float TargetWaitingTime;

	float CurrentWeatherIntensity;
	float TargetWeatherIntensity;
	float CurrentCloudOpacity;
	float TargetCloudOpacity;
};
