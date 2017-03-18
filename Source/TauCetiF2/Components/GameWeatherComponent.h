

#pragma once

#include "Components/ActorComponent.h"
#include "Game/Weather/WeatherSavingHelpers.h"
#include "GameWeatherComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAUCETIF2_API UGameWeatherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameWeatherComponent();

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | GameWeatherComponent")
		UCurrentWeatherState* currentWeatherState;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameWeatherComponent")
		void LoadFromCarrier(USaveGameCarrier* carrier, TArray<FText>& validationErrors);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameWeatherComponent")
		void SaveToCarrier(USaveGameCarrier* carrier);
	
};
