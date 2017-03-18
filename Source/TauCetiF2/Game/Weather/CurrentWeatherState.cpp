

#include "TauCetiF2.h"
#include "CurrentWeatherState.h"


UCurrentWeatherState::UCurrentWeatherState()
{
	CurrentWorldTime = -1;
}

void UCurrentWeatherState::MakeTargetsCurrent()
{
	CurrentWeatherIntensity = BaseWeatherIntensity = TargetWeatherIntensity;
	CurrentCloudOpacity = BaseCloudOpacity = TargetCloudOpacity;

}
