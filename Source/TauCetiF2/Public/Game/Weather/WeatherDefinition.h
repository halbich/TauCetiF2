#pragma once

#include "WeatherDefinition.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct TAUCETIF2_API FWeatherDefinition
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | Weather")
		bool ApplyDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | Weather")
		float MinDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | Weather")
		float MaxDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | Weather")
		float MinChangeDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | Weather")
		float MaxChangeDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | Weather")
		float MinCloudsSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | Weather")
		float MaxCloudsSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | Weather")
		float MinCloudsOpacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | Weather")
		float MaxCloudsOpacity;

	FWeatherDefinition()
	{
		ApplyDamage = false;

		MinDuration = 10;
		MaxDuration = 60;

		MinChangeDuration = 10;
		MaxChangeDuration = 60;

		MinCloudsSpeed = MaxCloudsSpeed = MinCloudsOpacity = MaxCloudsOpacity = 0.0f;
	}
};
