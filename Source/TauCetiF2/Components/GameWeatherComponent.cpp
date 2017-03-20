

#include "TauCetiF2.h"
#include "GameWeatherComponent.h"


// Sets default values for this component's properties
UGameWeatherComponent::UGameWeatherComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UGameWeatherComponent::LoadFromCarrier(USaveGameCarrier* carrier, TArray<FText>& validationErrors)
{
	check(carrier != NULL);

	if (!currentWeatherState)
		currentWeatherState = NewObject<UCurrentWeatherState>();

	WeatherSavingHelpers::FromWeatherState(currentWeatherState, carrier->weatherState);
}

void UGameWeatherComponent::SaveToCarrier(USaveGameCarrier* carrier)
{
	check(carrier != NULL);
	WeatherSavingHelpers::ToWeatherState(carrier->weatherState, currentWeatherState);
}



void UGameWeatherComponent::DEBUGShowMinMaxBoxes() {
	if (WeatherRootTree)
	{
		debugBoxesShown = true;
		WeatherRootTree->DEBUGDrawContainingBox(GetWorld());
	}
	else
		print(TEXT("NO Root!"));
}

void UGameWeatherComponent::DEBUGHideMinMaxBoxes() {
	if (WeatherRootTree)
	{
		FlushPersistentDebugLines(GetWorld());
		debugBoxesShown = false;
		//
	}
	else
		print(TEXT("NO Root!"));
}

void UGameWeatherComponent::ObjectsChanged() {
	if (debugBoxesShown)
	{
		FlushPersistentDebugLines(GetWorld());
		WeatherRootTree->DEBUGDrawContainingBox(GetWorld());
	}

}