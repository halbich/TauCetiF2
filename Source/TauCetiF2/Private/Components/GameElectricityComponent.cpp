

#include "TauCetiF2.h"
#include "GameElectricityComponent.h"

#pragma optimize("", off)

const double UGameElectricityComponent::maxFloatingTime(1.0 / 30.0);		// we want to have at least 30 FPS

// Sets default values for this component's properties

UGameElectricityComponent::UGameElectricityComponent() : Super(), networksToUpdate()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}


// Called when the game starts
void UGameElectricityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UGameElectricityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto time = FPlatformTime::Seconds() + maxFloatingTime;

	for (auto n : networks)
	{
		if (n->NetworkState == EElectricNetworkState::Invalid)
			continue;

		float totalElectricityAviable = 0.0f;

		for (auto producer : n->ElectricitySources)
			totalElectricityAviable += producer->ElectricityInfo->CurrentObjectEnergy;

		if (FMath::IsNearlyZero(totalElectricityAviable))	// we do net have enough power
			continue;
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

void UGameElectricityComponent::processNetwork(UElectricNetwork* network)
{
	UElectricityComponent* part;
	auto deq = network->ToRecompute.Dequeue(part);

	if (!deq || !part || !part->IsValidLowLevel() || part->IsPendingKill())
		return;

	ensure(part->ComponentNetworkState == EElectricNetworkState::InRecompute);

	part->ComponentNetworkState = EElectricNetworkState::Valid;

	// new block without connections
	if (part->ConnectedComponents.Num() == 0)
		return;

	// foreach connected item:

	//	mine network
	//		-	invalid				->	take for processing
	//		-	to be processed		->	nothing - will be processed
	//		-	valid				->	nothing

	//	other network
	//		-	invalid				->	steal ownership (removing) and take for processing
	//		-	to be processed		->	nothing - will be processed
	//		-	valid				->	merge networks, leaving one to be destroyed



	for (auto connected : part->ConnectedComponents)
	{
		auto areDifferent = connected->Network != part->Network;


		switch (connected->ComponentNetworkState)
		{
		case EElectricNetworkState::Invalid:

			if (areDifferent) {
				// we need to steal network
				auto removed = connected->Network->UnregisterEntity(connected);
				check(removed > 0);

				addToNetwork(connected, part->Network);
			}

			enqueueItem(connected);
			break;
		case EElectricNetworkState::InRecompute:
			break;
		case EElectricNetworkState::Valid:
			if (areDifferent) {
				// we need to merge networks. And we just say our si bigger since our is not pending delete and connected could be
				auto pn = part->Network;
				auto cn = connected->Network;
				mergeNetworks(pn, cn);
			}
			break;
		default:
			checkNoEntry();
			break;
		}
	}
}

void UGameElectricityComponent::RemoveFromWorldNetwork(UElectricityComponent* comp)
{
	ensure(comp && comp->Network);

	forceInvalidateNetwork(comp->Network);
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
