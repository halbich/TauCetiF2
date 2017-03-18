#pragma once

#include "CurrentWeatherState.h"
#include "GameSave/Public/Weather/FWeatherState.h"

/**
 *
 */

namespace WeatherSavingHelpers {
	static void FromWeatherState(UCurrentWeatherState* currentState, FWeatherState& weatherState) {
		currentState->CurrentDefinitionID = weatherState.CurrentDefinitionID;
		currentState->CurrentWaitingTime = weatherState.CurrentWaitingTime;
		currentState->TargetWaitingTime = weatherState.TargetWaitingTime;
		currentState->IsInWeatherChange = weatherState.IsInWeatherChange;
	}

	static void ToWeatherState(FWeatherState& weatherState, UCurrentWeatherState* currentState) {
		weatherState.CurrentDefinitionID = currentState->CurrentDefinitionID;
		weatherState.CurrentWaitingTime = currentState->CurrentWaitingTime;
		weatherState.TargetWaitingTime = currentState->TargetWaitingTime;
		weatherState.IsInWeatherChange = currentState->IsInWeatherChange;
	}
}