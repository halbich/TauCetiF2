#include "Commons.h"
#include "CommonHelpers.h"

const FString UCommonHelpers::customSectionName = TEXT("TCF2.CustomSettings");

FString UCommonHelpers::getConfigVariableNameFor(EGameUserSettingsVariable settings)
{
	switch (settings)
	{
	case EGameUserSettingsVariable::MasterVolume:	return TEXT("MasterVolume");
	case EGameUserSettingsVariable::MusicVolume:	return TEXT("MusicVolume");
	case EGameUserSettingsVariable::OtherVolume:	return TEXT("OtherVolume");
	case EGameUserSettingsVariable::GeneratorVisualizationEnabled:		return TEXT("GeneratorVisualizationEnabled");
	case EGameUserSettingsVariable::UseCulture:		return TEXT("UseCulture");

	default:
		checkNoEntry();
		return TEXT("");
	}
}

bool UCommonHelpers::GetSettingsValueFloat(EGameUserSettingsVariable settings, float& value)
{
	if (!GConfig)
		return false;

	auto varName = UCommonHelpers::getConfigVariableNameFor(settings);

	return GConfig->GetFloat(
		*UCommonHelpers::customSectionName,
		*varName,
		value,
		GGameIni
	);
}

bool UCommonHelpers::SetSettingsValueFloat(EGameUserSettingsVariable settings, float value)
{
	if (!GConfig)
		return false;

	auto varName = UCommonHelpers::getConfigVariableNameFor(settings);

	GConfig->SetFloat(
		*UCommonHelpers::customSectionName,
		*varName,
		value,
		GGameIni
	);

	GConfig->Flush(false, GGameIni);

	return true;
}

bool UCommonHelpers::GetSettingsValueBool(EGameUserSettingsVariable settings, bool& value)
{
	if (!GConfig)
		return false;

	auto varName = UCommonHelpers::getConfigVariableNameFor(settings);

	return GConfig->GetBool(
		*UCommonHelpers::customSectionName,
		*varName,
		value,
		GGameIni
	);
}

bool UCommonHelpers::SetSettingsValueBool(EGameUserSettingsVariable settings, bool value)
{
	if (!GConfig)
		return false;

	auto varName = UCommonHelpers::getConfigVariableNameFor(settings);

	GConfig->SetBool(
		*UCommonHelpers::customSectionName,
		*varName,
		value,
		GGameIni
	);

	GConfig->Flush(false, GGameIni);

	return true;
}

bool UCommonHelpers::GetSettingsValueString(EGameUserSettingsVariable settings, FString& value)
{
	if (!GConfig)
		return false;

	auto varName = UCommonHelpers::getConfigVariableNameFor(settings);

	return GConfig->GetString(
		*UCommonHelpers::customSectionName,
		*varName,
		value,
		GGameIni
	);
}

bool UCommonHelpers::SetSettingsValueString(EGameUserSettingsVariable settings, FString value)
{
	if (!GConfig)
		return false;

	auto varName = UCommonHelpers::getConfigVariableNameFor(settings);

	GConfig->SetString(
		*UCommonHelpers::customSectionName,
		*varName,
		*value,
		GGameIni
	);

	GConfig->Flush(false, GGameIni);

	return true;
}