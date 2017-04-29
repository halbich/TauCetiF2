#include "TauCetiF2.h"
#include "GameElectricityComponent.h"

#pragma optimize("", off)

const double UGameElectricityComponent::maxFloatingTime(1.0 / 30.0);		// we want to have at least 30 FPS

// Sets default values for this component's properties

UGameElectricityComponent::UGameElectricityComponent() : Super(), networksToUpdate(), SectionLock()
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

	SectionLock.Lock();

	if (!networksToUpdate.IsEmpty())
		tickRecomputeNetwork(time);

	SectionLock.Unlock();
}

void UGameElectricityComponent::AddToWorldNetwork(UElectricityComponent* comp)
{
	SectionLock.Lock();

	ensure(!comp->Network);

	auto n = addToNetwork(comp, NewObject<UElectricNetwork>());
	networks.AddUnique(n);

	enqueueItem(comp);

	networksToUpdate.Enqueue(n);

	SectionLock.Unlock();
}

void UGameElectricityComponent::RemoveFromWorldNetwork(UElectricityComponent* comp)
{
	SectionLock.Lock();
	ensure(comp && comp->Network);

	FString w;
	switch (comp->ComponentNetworkState)
	{
	case EElectricNetworkState::Invalid: w = TEXT("Invalid"); break;
	case EElectricNetworkState::InRecompute:  w = TEXT("InRecompute"); break;
	case EElectricNetworkState::Valid:  w = TEXT("Valid"); break;

	}

	UE_LOG(LogTemp, Log, TEXT("Removing for %s from %s, state: %s"), *comp->BlockInfo->GetName(), *comp->Network->GetName(), *w);

	comp->Network->ForceInvalidateNetwork();
	comp->Network->UnregisterEntity(comp);
	networksTodelete.AddUnique(comp->Network);

	for (auto connected : comp->ConnectedComponents)
	{
		connected->Network->UnregisterEntity(connected);

		auto n = addToNetwork(connected, NewObject<UElectricNetwork>());
		networks.AddUnique(n);
		enqueueItem(connected);
		networksToUpdate.Enqueue(n);
	}


	SectionLock.Unlock();
}

#pragma optimize("", on)