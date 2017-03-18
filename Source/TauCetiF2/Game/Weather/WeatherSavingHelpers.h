#pragma once

#include "CurrentWeatherState.h"
#include "GameSave/Public/Weather/FWeatherState.h"

/**
 *
 */

namespace WeatherSavingHelpers {
	static void FromWeatherState(UCurrentWeatherState* currentState, FWeatherState& weatherState) {
		currentState->CurrentDefinitionID = weatherState.CurrentDefinitionID;
		currentState->CurrentWorldTime = weatherState.CurrentWorldTime;
		currentState->CurrentWaitingTime = weatherState.CurrentWaitingTime;
		currentState->IsInWeatherChange = weatherState.IsInWeatherChange;
	}

	static void ToWeatherState(FWeatherState& weatherState, UCurrentWeatherState* currentState) {
		weatherState.CurrentDefinitionID = currentState->CurrentDefinitionID;
		weatherState.CurrentWorldTime = currentState->CurrentWorldTime;
		weatherState.CurrentWaitingTime = currentState->CurrentWaitingTime;
		weatherState.IsInWeatherChange = currentState->IsInWeatherChange;
	}
}