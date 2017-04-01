

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

	float BaseWeatherIntensity;
	float CurrentWeatherIntensity;
	float TargetWeatherIntensity;
	
	float BaseCloudOpacity;
	float CurrentCloudOpacity;
	float TargetCloudOpacity;


	float HitpointsCounter;
	float EaseIn;
	float EaseOut;
	uint8 StormState;

};
