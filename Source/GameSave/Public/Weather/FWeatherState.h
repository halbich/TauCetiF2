

#pragma once

/**
 *
 */
struct GAMESAVE_API FWeatherState
{
public:
	FWeatherState();

	int32 CurrentDefinitionID;
	float CurrentWorldTime;
	float CurrentWaitingTime;
	bool  IsInWeatherChange;
};
