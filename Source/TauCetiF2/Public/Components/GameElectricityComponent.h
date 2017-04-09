

#pragma once

#include "Components/ActorComponent.h"
#include "Blocks/Public/Components/ElectricityComponent.h"
#include "Commons/Public/Enums.h"
#include "GameElectricityComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API UGameElectricityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameElectricityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TQueue<UElectricNetwork*> networksToUpdate;

	UPROPERTY(Transient)
		TArray<UElectricNetwork*> networksTodelete;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void AddToWorldNetwork(UElectricityComponent* comp);

	void RemoveFromWorldNetwork(UElectricityComponent* comp);

private:

	void processNetwork(UElectricNetwork* network);

	static const double maxFloatingTime;


	FORCEINLINE void enqueueItem(UElectricityComponent* comp) {
		comp->ComponentNetworkState = EElectricNetworkState::InRecompute;
		comp->Network->ToRecompute.Enqueue(comp);
		comp->Network->NetworkState = EElectricNetworkState::InRecompute;
	}

	FORCEINLINE UElectricNetwork* addToNetwork(UElectricityComponent* comp, UElectricNetwork* network)
	{
		auto r = comp->Network = network;
		r->Entities.Add(comp);
		return r;
	}

	void mergeNetworks(UElectricNetwork* bigger, UElectricNetwork* smaller)
	{

		for (auto smEnt : smaller->Entities)
			addToNetwork(smEnt, bigger);

		smaller->Entities.Empty();

		UElectricityComponent* deq;
		while (smaller->ToRecompute.Dequeue(deq))
			enqueueItem(deq);

		smaller->MarkPendingKill();
	}


	void forceInvalidateNetwork(UElectricNetwork* net)
	{
		net->NetworkState = EElectricNetworkState::Invalid;
		for (auto netEnt : net->Entities)
			netEnt->ComponentNetworkState = EElectricNetworkState::Invalid;

		net->ToRecompute.Empty();
	}


};
