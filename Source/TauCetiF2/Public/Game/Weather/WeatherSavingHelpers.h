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

		currentState->BaseWeatherIntensity = weatherState.BaseWeatherIntensity;
		currentState->CurrentWeatherIntensity = weatherState.CurrentWeatherIntensity;
		currentState->TargetWeatherIntensity = weatherState.TargetWeatherIntensity;

		currentState->BaseCloudOpacity = weatherState.BaseCloudOpacity;
		currentState->CurrentCloudOpacity = weatherState.CurrentCloudOpacity;
		currentState->TargetCloudOpacity = weatherState.TargetCloudOpacity;
	}

	static void GetAdditionals(const FWeatherState& weatherState, float& hitpointsCounter, float& playerHitpointCounter, float& easeIn, float& easeOut, uint8& stormState)
	{
		hitpointsCounter = weatherState.HitpointsCounter;
		easeIn = weatherState.EaseIn;
		easeOut = weatherState.EaseOut;
		stormState = weatherState.StormState;
		playerHitpointCounter = weatherState.PlayerHitpointCounter;
	}

	static void ToWeatherState(FWeatherState& weatherState, UCurrentWeatherState* currentState) {
		weatherState.CurrentDefinitionID = currentState->CurrentDefinitionID;
		weatherState.CurrentWaitingTime = currentState->CurrentWaitingTime;
		weatherState.TargetWaitingTime = currentState->TargetWaitingTime;
		weatherState.IsInWeatherChange = currentState->IsInWeatherChange;

		weatherState.BaseWeatherIntensity = currentState->BaseWeatherIntensity;
		weatherState.CurrentWeatherIntensity = currentState->CurrentWeatherIntensity;
		weatherState.TargetWeatherIntensity = currentState->TargetWeatherIntensity;

		weatherState.BaseCloudOpacity = currentState->BaseCloudOpacity;
		weatherState.CurrentCloudOpacity = currentState->CurrentCloudOpacity;
		weatherState.TargetCloudOpacity = currentState->TargetCloudOpacity;
	}

	static void SetAdditionals(FWeatherState& weatherState, const float hitpointsCounter, const float playerHitpointCounter, const float easeIn, const float easeOut, const uint8 stormState)
	{
		weatherState.HitpointsCounter = hitpointsCounter;
		weatherState.EaseIn = easeIn;
		weatherState.EaseOut = easeOut;
		weatherState.StormState = stormState;
		weatherState.PlayerHitpointCounter = playerHitpointCounter;
	}
}