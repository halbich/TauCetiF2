#include "TauCetiF2.h"
#include "GameElectricityComponent.h"

#pragma optimize("", off)

const double UGameElectricityComponent::maxFloatingTime(1.0 / 30.0);		// we want to have at least 30 FPS

// Sets default values for this component's properties

UGameElectricityComponent::UGameElectricityComponent() : Super(), networksToUpdate()
{
	PrimaryComponentTick.bCanEverTick = true;

	CustomTickInterval = 0.1f;		// we want to tick 10x per second by default

	SetComponentTickInterval(CustomTickInterval);
}

void UGameElectricityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto time = FPlatformTime::Seconds() + maxFloatingTime;

	for (auto n : networks)
	{
		if (n->NetworkState == EElectricNetworkState::Invalid)
			continue;

		tickUpdateNetwork(n, DeltaTime);

	}


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