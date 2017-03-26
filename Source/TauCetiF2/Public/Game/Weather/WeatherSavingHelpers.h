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

		currentState->CurrentWeatherIntensity = weatherState.CurrentWeatherIntensity;
		currentState->TargetWeatherIntensity = weatherState.TargetWeatherIntensity;
		currentState->CurrentCloudOpacity = weatherState.CurrentCloudOpacity;
		currentState->TargetCloudOpacity = weatherState.TargetCloudOpacity;


	}

	static void ToWeatherState(FWeatherState& weatherState, UCurrentWeatherState* currentState) {
		weatherState.CurrentDefinitionID = currentState->CurrentDefinitionID;
		weatherState.CurrentWaitingTime = currentState->CurrentWaitingTime;
		weatherState.TargetWaitingTime = currentState->TargetWaitingTime;
		weatherState.IsInWeatherChange = currentState->IsInWeatherChange;

		weatherState.CurrentWeatherIntensity = currentState->CurrentWeatherIntensity;
		weatherState.TargetWeatherIntensity = currentState->TargetWeatherIntensity;
		weatherState.CurrentCloudOpacity = currentState->CurrentCloudOpacity;
		weatherState.TargetCloudOpacity = currentState->TargetCloudOpacity;

	}
}