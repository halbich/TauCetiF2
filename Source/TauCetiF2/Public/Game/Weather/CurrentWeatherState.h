

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
		bool IsInWeatherChange;


	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float CurrentWaitingTime;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float TargetWaitingTime;


	// This property is not serializable to the SaveGame
	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float BaseWeatherIntensity;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float CurrentWeatherIntensity;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float TargetWeatherIntensity;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float BaseCloudOpacity;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float CurrentCloudOpacity;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | Weather")
		float TargetCloudOpacity;


	UFUNCTION(BlueprintCallable, Category = "TCF2 | Weather")
		void MakeTargetsCurrent();



	UFUNCTION(BlueprintCallable, Category = "TCF2 | Weather")
		bool CheckTimeExceeded(UPARAM(REF)FWeatherDefinition& definition, int32 newDefinitionID);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Weather")
		bool CheckChangingWeather(UPARAM(REF)FWeatherDefinition& definition);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Weather")
		bool CheckRunningWeather(UPARAM(REF)FWeatherDefinition& definition, int32 newDefinitionID);

};
