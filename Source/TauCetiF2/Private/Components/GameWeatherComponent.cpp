#include "TauCetiF2.h"
#include "GameWeatherComponent.h"

const float UGameWeatherComponent::EasingBorderValue = 1.0f / 10.0f;

// Sets default values for this component's properties
UGameWeatherComponent::UGameWeatherComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WeatherRootTree = CreateDefaultSubobject<UWeatherTargetsKDTree>(TEXT("WeatherRootBox"));
	startPointZ = 1000; // ((GameDefinitions::WorldBorders + FVector(0, 0, 25)) *  GameDefinitions::CubeMinSize).Z;
}

#pragma optimize("", off)
void UGameWeatherComponent::LoadFromCarrier(USaveGameCarrier* carrier, TArray<FText>& validationErrors)
{
	check(carrier != NULL);

	if (!currentWeatherState)
		currentWeatherState = NewObject<UCurrentWeatherState>();

	WeatherSavingHelpers::FromWeatherState(currentWeatherState, carrier->weatherState);

	uint8 stormState;
	WeatherSavingHelpers::GetAdditionals(carrier->weatherState, hitpointsCounter, playerHitpointCounter, currentEaseInTime, currentEaseOutTime, stormState);
	StormState = (EStormState)stormState;
}



#pragma optimize("", on)

void UGameWeatherComponent::SaveToCarrier(USaveGameCarrier* carrier)
{
	check(carrier != NULL);
	WeatherSavingHelpers::ToWeatherState(carrier->weatherState, currentWeatherState);
	WeatherSavingHelpers::SetAdditionals(carrier->weatherState, hitpointsCounter, playerHitpointCounter, currentEaseInTime, currentEaseOutTime, (uint8)StormState);
}

void UGameWeatherComponent::DEBUGShowMinMaxBoxes() {
	if (WeatherRootTree && !debugBoxesShown)
	{
		debugBoxesShown = true;
		WeatherRootTree->DEBUGDrawContainingBox(GetWorld());
	}
}

void UGameWeatherComponent::DEBUGHideMinMaxBoxes() {
	if (WeatherRootTree && debugBoxesShown)
	{
		FlushPersistentDebugLines(GetWorld());
		debugBoxesShown = false;
	}
}

void UGameWeatherComponent::ObjectsChanged() {
	if (debugBoxesShown)
	{
		FlushPersistentDebugLines(GetWorld());
		WeatherRootTree->DEBUGDrawContainingBox(GetWorld());
	}
}

void UGameWeatherComponent::OnStormComming()
{
	StormState = EStormState::Comming;
}

void UGameWeatherComponent::OnStormBegin()
{
	if (StormState == EStormState::NoStorm)
	{
		// we need to plan next storm, otherwise it is loaded and planned
		currentWeatherState->CurrentWeatherIntensity = currentWeatherState->TargetWeatherIntensity;

		hitpointsCounter = 0;
		playerHitpointCounter = 0;

		currentEaseInTime = FMath::RandRange(1.0f, EasingBorderValue *currentWeatherState->TargetWaitingTime);	// we start at 1sec, so we don't divide by zero in a tickComp.
		currentEaseOutTime = FMath::RandRange((1.0f - EasingBorderValue)*currentWeatherState->TargetWaitingTime, currentWeatherState->TargetWaitingTime - 1.0f);		// again we need to avoid dividing by zero

		StormState = EStormState::EaseIn;
	}

	CurrentHitIntensity = IntensityCurve->GetFloatValue(currentWeatherState->CurrentWeatherIntensity);

	StormNotification.Broadcast(EStormNotification::StormBegin);
}

void UGameWeatherComponent::OnStormEnd(bool notifyEnd)
{
	auto remainingTargets = FMath::FloorToInt(hitpointsCounter);
	if (remainingTargets > 0)
		doDamage(remainingTargets);

	auto remainingPlayerTargets = FMath::FloorToInt(playerHitpointCounter);
	if (remainingPlayerTargets > 0)
		doPlayerDamage(remainingPlayerTargets);

	StormState = EStormState::NoStorm;

	CurrentHitIntensity = 0;

	currentEaseInTime = 0;
	currentEaseOutTime = 0;

	hitpointsCounter = 0;
	playerHitpointCounter = 0;

	if (notifyEnd)
		StormNotification.Broadcast(EStormNotification::StormEnd);
}

void UGameWeatherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TickingEnabled)
		return;

	if (StormState == EStormState::NoStorm)
		return;

	if (StormState == EStormState::Comming)
	{
		if (currentWeatherState->CurrentWaitingTime >= currentWeatherState->TargetWaitingTime - 30 * 60)	// we want to know 30 minutes in game seconds before notify
		{
			StormNotification.Broadcast(EStormNotification::StormIsComming);
			StormState = EStormState::NoStorm;
		}
		return;
	}

	auto currentSurface = Targets.Num() * GameDefinitions::CubeSurfaceInMetersSquared;

	currentEasingTime += DeltaTime;
	switch (StormState)
	{
	case EStormState::EaseIn: {
		auto lerpedIntensity = FMath::Lerp(0.0f, CurrentHitIntensity, currentWeatherState->CurrentWaitingTime / currentEaseInTime);

		auto hp = lerpedIntensity * DeltaTime;
		hitpointsCounter += hp * currentSurface;
		playerHitpointCounter += hp;

		if (currentWeatherState->CurrentWaitingTime > currentEaseInTime)
			StormState = EStormState::Running;

		break;
	}

	case EStormState::Running: {
		auto hp = CurrentHitIntensity * DeltaTime;
		hitpointsCounter += hp * currentSurface;
		playerHitpointCounter += hp;

		if (currentWeatherState->CurrentWaitingTime > currentEaseOutTime)
			StormState = EStormState::EaseOut;

		break;
	}

	case EStormState::EaseOut: {
		auto lerpedIntensity = FMath::Max(FMath::Lerp(CurrentHitIntensity, 0.0f, (currentWeatherState->CurrentWaitingTime - currentEaseOutTime) / (currentWeatherState->TargetWaitingTime - currentEaseOutTime)), .0f);

		auto hp = lerpedIntensity * DeltaTime;
		hitpointsCounter += hp * currentSurface;
		playerHitpointCounter += hp;

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

	auto remainingPlayerTargets = FMath::FloorToInt(playerHitpointCounter);
	if (remainingPlayerTargets > 0)
	{
		doPlayerDamage(remainingPlayerTargets);
		playerHitpointCounter -= remainingPlayerTargets;
	}
}

void UGameWeatherComponent::OnTargetElementsChanged(UWeatherTargetsKDTree* target, bool isAdding)
{
	if (isAdding)
		Targets.Add(target);
	else
	{
		auto cnt = Targets.Remove(target);
		ensure(cnt == 1);
	}
}

void UGameWeatherComponent::InitComp()
{
	WeatherRootTree->rootNode = WeatherRootTree->GetRoot();
	WeatherRootTree->OnWeatherTargetsChanged.AddDynamic(this, &UGameWeatherComponent::OnTargetElementsChanged);
}