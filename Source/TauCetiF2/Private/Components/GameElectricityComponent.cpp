#include "TauCetiF2.h"
#include "GameElectricityComponent.h"

#pragma optimize("", off)

const double UGameElectricityComponent::maxFloatingTime(1.0 / 30.0);		// we want to have at least 30 FPS

// Sets default values for this component's properties

UGameElectricityComponent::UGameElectricityComponent() : Super(), networksToUpdate()
{
	PrimaryComponentTick.bCanEverTick = true;

	UpdateInterval = 1.0f / 10.0f;
}

void UGameElectricityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto time = FPlatformTime::Seconds() + maxFloatingTime;

	TimeSinceLastRecompute += DeltaTime;
	TimeSinceLastUpdate += DeltaTime;

	for (auto n : networks)
	{
		if (n->NetworkState == EElectricNetworkState::Invalid)
			continue;

		if (TimeSinceLastUpdate >= UpdateInterval)
			tickUpdateNetwork(n);

		if (TimeSinceLastRecompute >= 0.5f)
			updateStatistics(n);
	}

	if (TimeSinceLastRecompute >= 0.5f)
		TimeSinceLastRecompute = 0.0f;

	if (TimeSinceLastUpdate >= UpdateInterval)
		TimeSinceLastUpdate -= UpdateInterval;

	if (networksToUpdate.IsEmpty())
		return;

	tickRecomputeNetwork(time);
}

void UGameElectricityComponent::AddToWorldNetwork(UElectricityComponent* comp)
{
	ensure(!comp->Network);

	auto n = addToNetwork(comp, NewObject<UElectricNetwork>());

	enqueueItem(comp);

	networksToUpdate.Enqueue(n);
}

void UGameElectricityComponent::RemoveFromWorldNetwork(UElectricityComponent* comp)
{
	ensure(comp && comp->Network);

	comp->Network->ForceInvalidateNetwork();
	comp->Network->UnregisterEntity(comp);

	for (auto connected : comp->ConnectedComponents)
	{
		connected->Network->UnregisterEntity(connected);

		auto n = addToNetwork(connected, NewObject<UElectricNetwork>());
		enqueueItem(connected);
		networksToUpdate.Enqueue(n);
	}

	networksTodelete.AddUnique(comp->Network);
}

#pragma optimize("", on)