

#include "TauCetiF2.h"
#include "GameWeatherComponent.h"

#pragma optimize("", off)


const float UGameWeatherComponent::OneSixth = 1.0f / 6.0f;

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
	hitpointsCounter = 0;

	currentEaseInTime = FMath::RandRange(OneSixth *currentWeatherState->TargetWaitingTime, 2 * OneSixth *currentWeatherState->TargetWaitingTime);
	currentEaseOutTime = FMath::RandRange(4 * OneSixth *currentWeatherState->TargetWaitingTime, 5 * OneSixth *currentWeatherState->TargetWaitingTime);

	StormState = EStormState::EaseIn;
}

void UGameWeatherComponent::OnStormEnd()
{
	auto remainingTargets = FMath::FloorToInt(hitpointsCounter);
	if (remainingTargets > 0)
		doDamage(remainingTargets);


	StormState = EStormState::NoStorm;

	CurrentHitIntensity = 0;

	currentEaseInTime = 0;
	currentEaseOutTime = 0;

	hitpointsCounter = 0;
}

void UGameWeatherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (StormState == EStormState::NoStorm)
		return;

	auto currentSurface = Targets.Num() * GameDefinitions::CubeSurfaceInMetersSquared;

	currentEasingTime += DeltaTime;
	switch (StormState)
	{

	case EStormState::EaseIn: {


		auto lerpedIntensity = FMath::Lerp(0.0f, CurrentHitIntensity, currentWeatherState->CurrentWaitingTime / currentEaseInTime);

		hitpointsCounter += lerpedIntensity * DeltaTime * currentSurface;

		if (currentWeatherState->CurrentWaitingTime > currentEaseInTime)
			StormState = EStormState::Running;

		break;
	}

	case EStormState::Running: {

		hitpointsCounter += CurrentHitIntensity * DeltaTime * currentSurface;
		
		if (currentWeatherState->CurrentWaitingTime > currentEaseOutTime)
			StormState = EStormState::EaseOut;

		break;
	}

	case EStormState::EaseOut: {

		auto lerpedIntensity = FMath::Max( FMath::Lerp(CurrentHitIntensity, 0.0f, (currentWeatherState->CurrentWaitingTime - currentEaseOutTime) / (currentWeatherState->TargetWaitingTime - currentEaseOutTime)), .0f);

		hitpointsCounter += lerpedIntensity * DeltaTime * currentSurface;

		break;
	}
	default:
		break;
	}

	auto remainingTargets = FMath::FloorToInt(hitpointsCounter);
	if (remainingTargets > 0)
	{
		doDamage(remainingTargets);
		hitpointsCounter -= remainingTargets;
	}

}

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