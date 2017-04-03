

#include "TauCetiF2.h"
#include "GameElectricityComponent.h"

#pragma optimize("", off)

// Sets default values for this component's properties
UGameElectricityComponent::UGameElectricityComponent()
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

	// ...
}

void UGameElectricityComponent::AddToWorldNetwork(UElectricityComponent* comp)
{


	if (comp->ConnectedComponents.Num() == 0)
	{
		// new block without connections
		comp->Network = NewObject<UElectricNetwork>();
		comp->Network->RegisterEntity(comp);
		comp->NetworkState = EElectricNetworkState::Valid;

		return;
	}

	TMap<UElectricNetwork*, TArray< UElectricityComponent*>> updateItems;
	for (auto c : comp->ConnectedComponents)
	{
		auto target = updateItems.FindOrAdd(c->Network);
		target.Add(c);
	}

	if (updateItems.Num() == 1)	// we are adding to single network
	{
		for (auto n : updateItems)
		{
			comp->Network = n.Key;
			comp->Network->RegisterEntity(comp);
			comp->NetworkState = EElectricNetworkState::Valid;
			//comp->Network->SetState(EElectricNetworkState::Valid);
		}

		return;
	}

	for (auto n : updateItems)
	{
		comp->Network = n.Key;
		comp->Network->RegisterEntity(comp);
		comp->NetworkState = EElectricNetworkState::Valid;
		//comp->Network->SetState(EElectricNetworkState::Valid);
	}

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
}

#pragma optimize("", on)
