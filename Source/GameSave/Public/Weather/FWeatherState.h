

#pragma once

/**
 *
 */
struct GAMESAVE_API FWeatherState
{
public:
	FWeatherState();

	int32 CurrentDefinitionID;
	float CurrentWaitingTime;
	float TargetWaitingTime;
	bool  IsInWeatherChange;
};
