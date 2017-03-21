

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
	// we want to be sure that the weather changing simulation is done and completed
	check(currentWeatherState->TargetWeatherIntensity == currentWeatherState->CurrentWeatherIntensity);

	CurrentHitIntensity = IntensityCurve->GetFloatValue(currentWeatherState->CurrentWeatherIntensity);
	currentEasingTime = 0;
	hitpointsCounter = 0;
	
	StormState = EStormState::EaseIn;
}

void UGameWeatherComponent::OnStormEnd()
{
	StormState = EStormState::NoStorm;

	CurrentHitIntensity = 0;
	currentEasingTime = 0;
	hitpointsCounter = 0;
}

void UGameWeatherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (StormState == EStormState::NoStorm)
		return;

	// TODO implementovat algoritmus DeltaTime * intensity * aktuální velikost plochy v m^2 - z toho poèítat hitpointy. to je pak poèet náhodných výbìrù ze senamu kandidátù

	switch (StormState)
	{

	case EStormState::EaseIn: {

		currentEasingTime += DeltaTime;

		auto lerpedIntensity = EntryEasingTime > 0 ? FMath::Lerp(0.0f, CurrentHitIntensity, currentEasingTime / EntryEasingTime) : CurrentHitIntensity;

		hitpointsCounter += lerpedIntensity * DeltaTime;

		if (currentEasingTime > EntryEasingTime)
		{
			StormState = EStormState::Running;
			currentEasingTime = 0;
		}

		break;
	}

	case EStormState::Running: {

		hitpointsCounter += CurrentHitIntensity * DeltaTime;

		auto remaining = currentWeatherState->TargetWaitingTime - currentWeatherState->CurrentWaitingTime;
		if (remaining < EntryEasingTime)
		{
			StormState = EStormState::EaseOut;
			currentEasingTime = EntryEasingTime - remaining;
		}

		break;
	}

	case EStormState::EaseOut: {

		currentEasingTime += DeltaTime;

		auto lerpedIntensity = EntryEasingTime > 0 ?  FMath::Lerp(CurrentHitIntensity, 0.0f, currentEasingTime / EntryEasingTime) : CurrentHitIntensity;

		hitpointsCounter += lerpedIntensity * DeltaTime;

		break;
	}
	default:
		break;
	}

	auto remainingTargets = FMath::FloorToInt(hitpointsCounter);
	if(remainingTargets > 0)
	{
		doDamage(remainingTargets);
		hitpointsCounter -= remainingTargets;
	}

	print0(*FString::FromInt(CurrentHitIntensity));




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