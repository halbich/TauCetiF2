#pragma once

#include "UObject/NoExportTypes.h"
#include "Enums.h"
#include "CommonHelpers.generated.h"

/**
 *
 */
UCLASS()
class COMMONS_API UCommonHelpers : public UObject
{
	GENERATED_BODY()

private:
	static const FString customSectionName;

	static FString getConfigVariableNameFor(EGameUserSettingsVariable settings);

public:

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers | Settings")
		static bool GetSettingsValueFloat(EGameUserSettingsVariable settings, float& value);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers | Settings")
		static bool SetSettingsValueFloat(EGameUserSettingsVariable settings, float value);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers | Settings")
		static bool GetSettingsValueBool(EGameUserSettingsVariable settings, bool& value);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers | Settings")
		static bool SetSettingsValueBool(EGameUserSettingsVariable settings, bool value);
};
