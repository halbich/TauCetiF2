

#pragma once

#include "UObject/NoExportTypes.h"
#include "CurrentWeatherState.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UCurrentWeatherState : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, Category = "TCF2 | Weather")
		int32 CurrentDefinitionID;
	
	
	UPROPERTY(BlueprintReadWrite, Category = "TCF2 | Weather")
		float CurrentWorldTime;

	UPROPERTY(BlueprintReadWrite, Category = "TCF2 | Weather")
		float CurrentWaitingTime;

	UPROPERTY(BlueprintReadWrite, Category = "TCF2 | Weather")
		bool IsInWeatherChange;
};
