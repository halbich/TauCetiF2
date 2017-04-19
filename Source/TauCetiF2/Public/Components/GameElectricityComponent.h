#pragma once

#include "Components/ActorComponent.h"
#include "Blocks/Public/Components/ElectricityComponent.h"
#include "Blocks/Public/Components/Electricity/ElectricNetwork.h"
#include "Commons/Public/Enums.h"
#include "GameElectricityComponent.generated.h"


#pragma optimize("", off)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API UGameElectricityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGameElectricityComponent();

protected:

	TQueue<UElectricNetwork*> networksToUpdate;

	UPROPERTY(Transient)
		TArray<UElectricNetwork*> networksTodelete;

	UPROPERTY(Transient)
		TArray<UElectricNetwork*> networks;

	UPROPERTY(Transient)
		float TimeSinceLastRecompute;


public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddToWorldNetwork(UElectricityComponent* comp);

	void RemoveFromWorldNetwork(UElectricityComponent* comp);

private:

	static const double maxFloatingTime;

	FORCEINLINE void enqueueItem(UElectricityComponent* comp) {
		comp->ComponentNetworkState = EElectricNetworkState::InRecompute;
		comp->Network->ToRecompute.Enqueue(comp);
		comp->Network->NetworkState = EElectricNetworkState::InRecompute;
	}

	/*FORCEINLINE*/ UElectricNetwork* addToNetwork(UElectricityComponent* comp, UElectricNetwork* network)
	{
		networks.AddUnique(network);
		auto r = comp->Network = network;
		r->RegisterEntity(comp);
		return r;
	}

	FORCEINLINE void mergeNetworks(UElectricNetwork* bigger, UElectricNetwork* smaller)
	{
		ensure(!networksTodelete.Contains(bigger));

		for (auto smEnt : smaller->Entities)
			addToNetwork(smEnt, bigger);


		UElectricityComponent* deq;
		while (smaller->ToRecompute.Dequeue(deq))
			enqueueItem(deq);

		smaller->EmptyNetwork();

		networks.Remove(smaller);
		smaller->MarkPendingKill();
	}

	FORCEINLINE void forceInvalidateNetwork(UElectricNetwork* net)
	{
		net->NetworkState = EElectricNetworkState::Invalid;
		for (auto netEnt : net->Entities)
			netEnt->ComponentNetworkState = EElectricNetworkState::Invalid;

		net->ToRecompute.Empty();
	}

	/*FORCEINLINE*/ void tickUpdateNetwork(UElectricNetwork* n)
	{

		float totalElectricityAviable = 0.0f;

		for (auto producer : n->ElectricityProducers)
			totalElectricityAviable += producer->ElectricityInfo->CurrentObjectEnergy;

		n->TotalElectricityAviable = totalElectricityAviable;

		if (FMath::IsNearlyZero(totalElectricityAviable))	// we do net have enough power
		{
			n->TotalElectricityAviableFilling = 0;
		}
		else
		{
			n->TotalElectricityAviableFilling = n->TotalElectricityAviable / n->MaxElectricityAviable;

			float criticalAviable = 0.5f * totalElectricityAviable;

			totalElectricityAviable -= criticalAviable;
			// TODO
			float totalCriticalRequired = 0.0f;
			for (auto critical : n->CriticalRepairEntities)
			{
				auto owner = Cast<ABlock>(critical->GetOwner());
				ensure(owner);

				// TODO get (required, divide it by totalCritical required) and multiply by aviable
				//owner->Heal(...);
			}

			// TODO badly, to reapir

			for (auto critical : n->CriticalRepairEntities)
			{
				auto owner = Cast<ABlock>(critical->GetOwner());
				ensure(owner);

				// TODO get (required, divide it by totalCritical required) and multiply by aviable
				//	owner->HealthUpdated()
			}

			// TODO badly, to repair

			// TODO remaining energy fill to consumers

			if (n->ElectricityConsumers.Num() > 0 && totalElectricityAviable > 0) {
				auto electricityConsumed = 0.0f;

				auto totalRequired = 0.0f;
				//auto ration = totalElectricityAviable / n->ElectricityConsumers.Num();

				for (auto consumer : n->ElectricityConsumers)
					totalRequired += consumer->ElectricityInfo->CurrentObjectMaximumEnergy - consumer->ElectricityInfo->CurrentObjectEnergy;

				if (totalRequired > 0)
				{
					for (auto consumer : n->ElectricityConsumers)
					{
						ensure(consumer->ElectricityInfo->CurrentObjectEnergy >= 0 && consumer->ElectricityInfo->CurrentObjectEnergy <= consumer->ElectricityInfo->CurrentObjectMaximumEnergy);

						auto ration = (consumer->ElectricityInfo->CurrentObjectMaximumEnergy - consumer->ElectricityInfo->CurrentObjectEnergy) / totalRequired;
						ensure(ration >= 0 && ration <= 1);

						auto aviable = totalElectricityAviable * ration;

						auto actuallyPutted = 0.0f;
						if (consumer->PutAmount(aviable, actuallyPutted))
							electricityConsumed += actuallyPutted;
					}

					totalElectricityAviable -= electricityConsumed;
					// todo do smthing with the rest
				}
			}
		}
	}

	FORCEINLINE void processNetwork(UElectricNetwork* network)
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
					mergeNetworks(part->Network, connected->Network);
				}
				break;
			default:
				checkNoEntry();
				break;
			}
		}
	}

	/*FORCEINLINE*/ void tickRecomputeNetwork(double time) {
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
			for (auto toDel : networksTodelete)
			{
				networks.Remove(toDel);
				ensure(toDel->Entities.Num() == 0);
				ensure(toDel->ToRecompute.IsEmpty());

				toDel->MarkPendingKill();
			}
			networksTodelete.Empty();

			for (auto n : networks)
				n->CheckControlBlocks();
		}
	}

	FORCEINLINE void updateStatistics(UElectricNetwork* n)
	{
		float producedEnergy = 0.0f;
		for (auto producer : n->ElectricityProducers)
		{
			producedEnergy += producer->EnergyProduced;
			producer->EnergyProduced = 0;
		}

		auto t = GameDefinitions::GameDayMultiplier * TimeSinceLastRecompute;

		n->EnergyProductionPerSec = producedEnergy / t;

		float consumedEnergy = 0.0f;
		for (auto consumer : n->ElectricityConsumers)
		{
			consumedEnergy += consumer->EnergyConsumed;
			consumer->EnergyConsumed = 0;
		}

		n->EnergyConsumptionPerSec = consumedEnergy / t;
	}
};



#pragma optimize("", on)