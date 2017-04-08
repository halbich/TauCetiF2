

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

		processNetwork(toResolve);

		if (!toResolve->ToRecompute.IsEmpty())	// we have another items to recompute
			networksToUpdate.Enqueue(toResolve);
		else
			toResolve->NetworkState = EElectricNetworkState::Valid;	

	} while (FPlatformTime::Seconds() <= time);

	if (networksToUpdate.IsEmpty())
		print(TEXT("Everything recomputed!"));

}

void UGameElectricityComponent::AddToWorldNetwork(UElectricityComponent* comp)
{
	ensure(!comp->Network);

	comp->ComponentNetworkState = EElectricNetworkState::InRecompute;

	auto net = NewObject<UElectricNetwork>();
	net->Entities.Add(comp);
	net->ToRecompute.Enqueue(comp);
	net->NetworkState = EElectricNetworkState::InRecompute;

	comp->Network = net;

	networksToUpdate.Enqueue(net);

}

void UGameElectricityComponent::processNetwork(UElectricNetwork* network)
{
	UElectricityComponent* part;
	auto deq = network->ToRecompute.Dequeue(part);
	ensure(deq && part);

	// todo check fro pending delete

	part->ComponentNetworkState = EElectricNetworkState::Valid;




	// new block without connections
	if (part->ConnectedComponents.Num() == 0)
		return;




	// foreach connected triage to:

	//	mine
	//		-	invalid				->	take for processing
	//		-	to be processed		->	nothing - will be processed
	//		-	valid				->	nothing

	//	other network
	//		-	invalid				->	steal ownership (removing) and take for processing
	//		-	to be processed		->	nothing - will be processed
	//		-	valid				->	merge networks, leaving one to be destroyed












	//TMap<UElectricNetwork*, TArray< UElectricityComponent*>> updateItems;
	//for (auto c : comp->ConnectedComponents)
	//{
	//	auto target = updateItems.FindOrAdd(c->Network);
	//	target.Add(c);
	//}

	//if (updateItems.Num() == 1)	// we are adding to single network
	//{
	//	for (auto n : updateItems)
	//	{
	//		comp->Network = n.Key;
	//		comp->Network->RegisterEntity(comp);
	//		comp->NetworkState = EElectricNetworkState::Valid;
	//		//comp->Network->SetState(EElectricNetworkState::Valid);
	//	}

	//	return;
	//}

	//for (auto n : updateItems)
	//{
	//	comp->Network = n.Key;
	//	comp->Network->RegisterEntity(comp);
	//	comp->NetworkState = EElectricNetworkState::Valid;
	//	//comp->Network->SetState(EElectricNetworkState::Valid);
	//}

	// we have multipleNetworks

	/*updateItems.KeySort([](UElectricNetwork* n1, UElectricNetwork* n2) {
		return n1->entities.Num() < n2->entities.Num();
	});*/

	print(TEXT("d"));

}

void UGameElectricityComponent::RemoveFromWorldNetwork(UElectricityComponent* comp)
{
	ensure(comp && comp->Network);
	comp->Network->UnregisterEntity(comp);

	// force invalidate network

	// foreach connected block remove connection, create new network for them and plan them to recompute

	// if no connected networks, destroy network
}

#pragma optimize("", on)
