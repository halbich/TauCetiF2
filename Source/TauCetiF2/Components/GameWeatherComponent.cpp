

#include "TauCetiF2.h"
#include "GameWeatherComponent.h"


// Sets default values for this component's properties
UGameWeatherComponent::UGameWeatherComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WeatherRootTree = CreateDefaultSubobject<UWeatherTargetsKDTree>(TEXT("WeatherRootBox"));
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

void UGameWeatherComponent::OnStormBegin()
{

	IsInStorm = true;
}

void UGameWeatherComponent::OnStormEnd()
{
	IsInStorm = false;
}

void UGameWeatherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsInStorm)
		return;

	print0(*FString::FromInt(Targets.Num()));
}
#pragma optimize("", off)
void UGameWeatherComponent::OnTargetElementsChanged(UWeatherTargetsKDTree* target, bool isAdding)
{
	if (isAdding)
		Targets.Add(target);
	else
	{
		auto cnt = Targets.Remove(target);
		check(cnt == 1);
	}
}

void UGameWeatherComponent::InitComp()
{
	WeatherRootTree->rootNode = WeatherRootTree->GetRoot();
	WeatherRootTree->OnWeatherTargetsChanged.AddDynamic(this, &UGameWeatherComponent::OnTargetElementsChanged);

}

#pragma optimize("", on)