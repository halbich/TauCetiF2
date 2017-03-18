#include "GameSave.h"
#include "FWeatherState.h"

FWeatherState::FWeatherState() :
	CurrentDefinitionID(-1), CurrentWaitingTime(-1), TargetWaitingTime(-1), IsInWeatherChange(false),
	CurrentWeatherIntensity(0), TargetWeatherIntensity(0), CurrentCloudOpacity(0), TargetCloudOpacity(0)

{
}