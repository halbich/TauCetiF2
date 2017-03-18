

#pragma once

#include "UObject/NoExportTypes.h"
#include "CurrentWeatherState.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class TAUCETIF2_API UCurrentWeatherState : public UObject
{
	GENERATED_BODY()
public:

	UCurrentWeatherState();

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		int32 CurrentDefinitionID;

	// This property is not serializable to the SaveGame
	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float CurrentWorldTime;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float CurrentWaitingTime;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float TargetWaitingTime;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		bool IsInWeatherChange;
};
