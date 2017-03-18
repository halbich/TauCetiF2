

#pragma once

#include "WeatherDefinition.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct TAUCETIF2_API FWeatherDefinition
{

	GENERATED_BODY();


	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Weather")
		uint8 DefinitionID;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Weather")
		bool ApplyDamage;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Weather")
		float MinDuration;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Weather")
		float MaxDuration;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Weather")
		float MinChangeDuration;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Weather")
		float MaxChangeDuration;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Weather")
		float MinCloudsSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Weather")
		float MaxCloudsSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Weather")
		float MinCloudsOpacity;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Weather")
		float MaxCloudsOpacity;

	FWeatherDefinition()
	{
		DefinitionID = 0;
		ApplyDamage = false;

		MinDuration = 10;
		MaxDuration = 60;

		MinChangeDuration = 10;
		MaxChangeDuration = 60;

		MinCloudsSpeed = MaxCloudsSpeed = MinCloudsOpacity = MaxCloudsOpacity = 0.0f;
	}
};
