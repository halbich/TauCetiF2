

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

	if (networksToUpdate.IsEmpty())
		return;

	auto time = FPlatformTime::Seconds() + maxFloatingTime;

	do {
		UElectricNetwork* toResolve;
		if (!networksToUpdate.Dequeue(toResolve))
			break;

		if (!toResolve || !toResolve->IsValidLowLevel() || toResolve->IsPendingKill())
			continue;

		processNetwork(toResolve);

		if (!toResolve || !toResolve->IsValidLowLevel() || toResolve->IsPendingKill())
			continue;

		if (!toResolve->ToRecompute.IsEmpty())	// we have another items to recompute
			networksToUpdate.Enqueue(toResolve);
		else
			toResolve->NetworkState = EElectricNetworkState::Valid;

	} while (FPlatformTime::Seconds() <= time);

	if (networksToUpdate.IsEmpty())
	{
		print(TEXT("Everything recomputed!"));

		for (auto toDel : networksTodelete)
		{
			ensure(toDel->Entities.Num() == 0);
			ensure(toDel->ToRecompute.IsEmpty());

			toDel->MarkPendingKill();
		}
	}

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
				auto removed = connected->Network->Entities.Remove(connected);
				check(removed > 0);

				addToNetwork(connected, part->Network);
			}

			enqueueItem(connected);
			break;
		case EElectricNetworkState::InRecompute:
			break;
		case EElectricNetworkState::Valid:
			if (areDifferent) {
				// we need to merge networks
				auto pn = part->Network;
				auto cn = connected->Network;
				if (pn->Entities.Num() >= cn->Entities.Num())
					mergeNetworks(pn, cn);
				else
					mergeNetworks(cn, pn);
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
	comp->Network->Entities.Remove(comp);


	for (auto connected : comp->ConnectedComponents)
	{
		connected->Network->Entities.Remove(connected);

		auto n = addToNetwork(connected, NewObject<UElectricNetwork>());
		enqueueItem(connected);
		networksToUpdate.Enqueue(n);
	}

	networksTodelete.AddUnique(comp->Network);
}

#pragma optimize("", on)
