#include "TauCetiF2.h"
#include "GameWeatherComponent.h"

#pragma optimize("", off)

const float UGameWeatherComponent::EasingBorderValue = 1.0f / 10.0f;

// Sets default values for this component's properties
UGameWeatherComponent::UGameWeatherComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WeatherRootTree = CreateDefaultSubobject<UWeatherTargetsKDTree>(TEXT("WeatherRootBox"));
	startPointZ = 1000; // ((GameDefinitions::WorldBorders + FVector(0, 0, 25)) *  GameDefinitions::CubeMinSize).Z;
}

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

void UGameWeatherComponent::SaveToCarrier(USaveGameCarrier* carrier)
{
	check(carrier != NULL);
	WeatherSavingHelpers::ToWeatherState(carrier->weatherState, currentWeatherState);
	WeatherSavingHelpers::SetAdditionals(carrier->weatherState, hitpointsCounter, playerHitpointCounter, currentEaseInTime, currentEaseOutTime, (uint8)StormState);
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
	currentWeatherState->CurrentWeatherIntensity = currentWeatherState->TargetWeatherIntensity;

	CurrentHitIntensity = IntensityCurve->GetFloatValue(currentWeatherState->CurrentWeatherIntensity);
	hitpointsCounter = 0;
	playerHitpointCounter = 0;

	currentEaseInTime = FMath::RandRange(1.0f, EasingBorderValue *currentWeatherState->TargetWaitingTime);	// we start at 1sec, so we don't divide by zero in a tickComp.
	currentEaseOutTime = FMath::RandRange((1.0f - EasingBorderValue)*currentWeatherState->TargetWaitingTime, currentWeatherState->TargetWaitingTime - 1.0f);		// again we need to avoid dividing by zero

	StormState = EStormState::EaseIn;
}

void UGameWeatherComponent::OnStormEnd()
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
		check(cnt == 1);
	}
}

void UGameWeatherComponent::InitComp()
{
	WeatherRootTree->rootNode = WeatherRootTree->GetRoot();
	WeatherRootTree->OnWeatherTargetsChanged.AddDynamic(this, &UGameWeatherComponent::OnTargetElementsChanged);
}

#pragma optimize("", on)