

#pragma once

#include "Components/ActorComponent.h"
#include "Blocks/Public/Components/ElectricityComponent.h"
#include "Blocks/Public/Components/Electricity/ElectricNetwork.h"
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

	UPROPERTY(Transient)
		TArray<UElectricNetwork*> networks;

	UPROPERTY(Transient)
		float TimeSinceLastRecompute;

	UPROPERTY(Transient)
		float dayMultiplier;

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
		networks.AddUnique(network);
		auto r = comp->Network = network;
		r->RegisterEntity(comp);
		return r;
	}

	void mergeNetworks(UElectricNetwork* bigger, UElectricNetwork* smaller)
	{
		ensure(!networksTodelete.Contains(bigger));

		for (auto smEnt : smaller->Entities)
			addToNetwork(smEnt, bigger);

		smaller->Entities.Empty();

		UElectricityComponent* deq;
		while (smaller->ToRecompute.Dequeue(deq))
			enqueueItem(deq);

		networks.Remove(smaller);
		smaller->MarkPendingKill();
	}


	void forceInvalidateNetwork(UElectricNetwork* net)
	{
		net->NetworkState = EElectricNetworkState::Invalid;
		for (auto netEnt : net->Entities)
			netEnt->ComponentNetworkState = EElectricNetworkState::Invalid;

		net->ToRecompute.Empty();
	}


	FORCEINLINE void tickRecomputeNetwork(double time) {
		do {
			UElectricNetwork* toResolve;
			if (!networksToUpdate.Dequeue(toResolve))
				break;

			if (!toResolve || !toResolve->IsValidLowLevel() || toResolve->IsPendingKill())
				continue;

			if (networksTodelete.Contains(toResolve))
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
				networks.Remove(toDel);
				ensure(toDel->Entities.Num() == 0);
				ensure(toDel->ToRecompute.IsEmpty());

				toDel->MarkPendingKill();
			}
			networksTodelete.Empty();

			print(*FText::AsNumber(networks.Num()).ToString());
		}
	}


};
