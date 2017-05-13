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

bool UCurrentWeatherState::CheckTimeExceeded(UPARAM(REF)FWeatherDefinition& definition, int32 newDefinitionID)
{
	CurrentDefinitionID = newDefinitionID;
	CurrentWaitingTime = 0;
	ApplyDamage = definition.ApplyDamage;

	TargetWaitingTime = FMath::RandRange(definition.MinChangeDuration, definition.MaxChangeDuration);
	TargetWeatherIntensity = FMath::RandRange(definition.MinCloudsSpeed, definition.MaxCloudsSpeed);
	TargetCloudOpacity = FMath::RandRange(definition.MinCloudsOpacity, definition.MaxCloudsOpacity);

	return definition.ApplyDamage;
}

bool UCurrentWeatherState::CheckChangingWeather(UPARAM(REF)FWeatherDefinition& definition)
{
	CurrentWaitingTime = 0;
	IsInWeatherChange = false;

	TargetWaitingTime = FMath::RandRange(definition.MinDuration, definition.MaxDuration);

	MakeTargetsCurrent();

	return definition.ApplyDamage;
}

bool UCurrentWeatherState::CheckRunningWeather(UPARAM(REF)FWeatherDefinition& definition, int32 newDefinitionID)
{
	CurrentDefinitionID = newDefinitionID;
	ApplyDamage = definition.ApplyDamage;

	TargetWaitingTime = FMath::RandRange(definition.MinDuration, definition.MaxDuration);
	TargetWeatherIntensity = FMath::RandRange(definition.MinCloudsSpeed, definition.MaxCloudsSpeed);
	TargetCloudOpacity = FMath::RandRange(definition.MinCloudsOpacity, definition.MaxCloudsOpacity);

	MakeTargetsCurrent();

	return definition.ApplyDamage;
}