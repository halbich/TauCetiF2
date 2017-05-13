#include "GameSave.h"
#include "FWeatherState.h"

FWeatherState::FWeatherState() :
	CurrentDefinitionID(-1), CurrentWaitingTime(-1), TargetWaitingTime(-1), IsInWeatherChange(false),
	BaseWeatherIntensity(0), CurrentWeatherIntensity(0), TargetWeatherIntensity(0),
	BaseCloudOpacity(0), CurrentCloudOpacity(0), TargetCloudOpacity(0),
	HitpointsCounter(0), PlayerHitpointCounter(0), EaseIn(0), EaseOut(0), StormState(0)

{
}