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


	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | GameElectricityComponent")
		float CustomTickInterval;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | GameElectricityComponent")
		float Max;

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

	FORCEINLINE UElectricNetwork* addToNetwork(UElectricityComponent* comp, UElectricNetwork* network)
	{
		networks.AddUnique(network);
		auto r = comp->Network = network;
		r->RegisterEntity(comp);
		return r;
	}

	/*FORCEINLINE*/ void mergeNetworks(UElectricNetwork* bigger, UElectricNetwork* smaller)
	{
		ensure(!networksTodelete.Contains(bigger));

		for (auto smEnt : smaller->Entities)
			addToNetwork(smEnt, bigger);

		UElectricityComponent* deq;
		while (smaller->ToRecompute.Dequeue(deq))
			enqueueItem(deq);

		smaller->EmptyNetwork();

		auto rem = networks.Remove(smaller);
		ensure(rem > 0);
		smaller->MarkPendingKill();
	}

	/*FORCEINLINE*/ void doHealing(UElectricNetwork* n, float& maxAviable)
	{
		float totalElectricityAviable = maxAviable;

		float criticalAviable = 0.5f * totalElectricityAviable;

		totalElectricityAviable -= criticalAviable;

		// TODO
		float criticalHealth = 0.0f;
		for (auto critical : n->CriticalRepairEntities)
		{
			if (critical->ComponentNetworkState == EElectricNetworkState::Valid)
				criticalHealth += critical->BlockInfo->Health;
			/*auto owner = Cast<ABlock>(critical->GetOwner());
			ensure(owner);
*/
// TODO get (required, divide it by totalCritical required) and multiply by aviable
//owner->Heal(...);
		}

		n->CriticalRepairHealth = criticalHealth;
		n->CriticalRepairHealthPercentage = FMath::IsNearlyZero(n->CriticalRepairMaxHealth) ? 0 : criticalHealth / n->CriticalRepairMaxHealth;

		// TODO badly, to reapir

		float importantHealth = 0.0f;
		for (auto important : n->ImportantRepairEntities)
		{
			if (important->ComponentNetworkState == EElectricNetworkState::Valid)
				importantHealth += important->BlockInfo->Health;

			auto owner = Cast<ABlock>(important->GetOwner());
			ensure(owner);


			// TODO get (required, divide it by totalCritical required) and multiply by aviable
			//	owner->HealthUpdated()
		}

		n->ImportantRepairHealth = importantHealth;
		n->ImportantRepairHealthPercentage = FMath::IsNearlyZero(n->ImportantRepairMaxHealth) ? 0 : importantHealth / n->ImportantRepairMaxHealth;


		float repairHealth = 0.0f;
		for (auto toRepair : n->ToRepairEntities)
		{
			if (toRepair->ComponentNetworkState == EElectricNetworkState::Valid)
				repairHealth += toRepair->BlockInfo->Health;

			/*auto owner = Cast<ABlock>(toRepair->GetOwner());
			ensure(owner);*/

			// TODO get (required, divide it by totalCritical required) and multiply by aviable
			//	owner->HealthUpdated()
		}

		n->ToRepairHealth = repairHealth;
		n->ToRepairHealthPercentage = FMath::IsNearlyZero(n->ToRepairMaxHealth) ? 0 : repairHealth / n->ToRepairMaxHealth;

	}



	/*FORCEINLINE*/ void tickUpdateNetwork(UElectricNetwork* n, float deltaTime)
	{
		auto t = GameDefinitions::GameDayMultiplier * deltaTime;

		float producedEnergy = 0.0f;
		// aviable from generators
		float producersEnergyAviable = 0.0f;
		for (auto producer : n->ElectricityProducers)
		{
			if (producer->ComponentNetworkState == EElectricNetworkState::Valid)
				producersEnergyAviable += producer->ElectricityInfo->CurrentObjectEnergy;

			producedEnergy += producer->EnergyProduced;
			producer->EnergyProduced = 0;
		}

		n->ProducersEnergyAviable = producersEnergyAviable;
		n->ProducersEnergyAviableFilling = FMath::IsNearlyZero(n->ProducersEnergyMaxAviable) ? 0 : producersEnergyAviable / n->ProducersEnergyMaxAviable;
		n->EnergyProductionPerSec = producedEnergy / t;

		// aviable from batteries
		float storagesEnergyAviable = 0.0f;
		for (auto storage : n->ElectricityStorages)
			if (storage->ComponentNetworkState == EElectricNetworkState::Valid)
				storagesEnergyAviable += storage->ElectricityInfo->CurrentObjectEnergy;

		n->StoragesEnergyAviable = storagesEnergyAviable;
		n->StoragesEnergyAviableFilling = FMath::IsNearlyZero(n->StoragesEnergyMaxAviable) ? 0 : storagesEnergyAviable / n->StoragesEnergyMaxAviable;


		float consumedEnergy = 0.0f;
		// aviable from consumers
		auto consumersEnergyAviable = 0.0f;
		for (auto consumer : n->ElectricityConsumers)
		{
			if (consumer->ComponentNetworkState == EElectricNetworkState::Valid)
				consumersEnergyAviable += consumer->ElectricityInfo->CurrentObjectEnergy;

			consumedEnergy += consumer->EnergyConsumed;
			consumer->EnergyConsumed = 0;
		}
		n->ConsumersEnergyAviable = consumersEnergyAviable;
		n->ConsumersEnergyAviableFilling = FMath::IsNearlyZero(n->ConsumersEnergyMaxAviable) ? 0 : consumersEnergyAviable / n->ConsumersEnergyMaxAviable;
		n->EnergyConsumptionPerSec = consumedEnergy / t;

		// total health
		auto totalHealth = 0.0f;
		for (auto ent : n->Entities)
			if (ent->ComponentNetworkState == EElectricNetworkState::Valid)
				totalHealth += ent->BlockInfo->Health;

		n->NetworkHealth = totalHealth;
		n->NetworkHealthPercentage = FMath::IsNearlyZero(n->NetworkMaxHealth) ? 0 : totalHealth / n->NetworkMaxHealth;


		auto maxAviable = producersEnergyAviable + storagesEnergyAviable;


		if (maxAviable > 0)
			doHealing(n, maxAviable);

		auto inRec = n->NetworkState == EElectricNetworkState::InRecompute;

		auto p = n->ProducersCount;
		auto pc = p + n->StoragesCount;

		auto cc = n->ConsumersCount;
		if (cc > 0 && pc > 0 && maxAviable > 0)
		{
			for (int32 i = 0; i < cc; i++)
			{
				auto ind = FMath::RandHelper(cc);
				ensure(n->ElectricityConsumers.IsValidIndex(ind));

				auto consElem = n->ElectricityConsumers[ind];
				if (inRec && consElem->ComponentNetworkState != EElectricNetworkState::Valid)
					continue;

				auto required = FMath::Max(0.0f, consElem->ElectricityInfo->CurrentObjectMaximumEnergy - consElem->ElectricityInfo->CurrentObjectEnergy);

				if (FMath::IsNearlyZero(required))
					continue;

				auto producerSelect = FMath::RandHelper(pc);

				UElectricityComponent* elem;
				if (producerSelect < p)
				{
					// we are in producers part
					ensure(n->ElectricityProducers.IsValidIndex(producerSelect));

					elem = n->ElectricityProducers[producerSelect];
					if (inRec && elem->ComponentNetworkState != EElectricNetworkState::Valid)
						continue;
				}
				else
				{
					producerSelect -= p;

					// we are in storage part
					ensure(n->ElectricityStorages.IsValidIndex(producerSelect));

					elem = n->ElectricityStorages[producerSelect];
					if (inRec && elem->ComponentNetworkState != EElectricNetworkState::Valid)
						continue;

				}

				float actuallyPutted = 0;
				float actuallyObtained = 0;
				float actuallyReturned = 0;
				if (elem->ObtainAmount(required, actuallyObtained))
				{
					if (consElem->PutAmount(actuallyObtained, actuallyPutted))
					{
						actuallyObtained -= actuallyPutted;
						maxAviable = FMath::Max(0.0f, maxAviable - actuallyPutted);
					}

					elem->PutAmount(actuallyObtained, actuallyReturned);
				}


			}


		}

	}

	/*FORCEINLINE*/ void processNetwork(UElectricNetwork* network)
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
				auto rem = networks.Remove(toDel);
				ensure(rem > 0);
				ensure(toDel->Entities.Num() == 0);
				ensure(toDel->ToRecompute.IsEmpty());

				toDel->MarkPendingKill();
			}
			networksTodelete.Empty();

			for (auto n : networks)
				n->CheckControlBlocks();

			SetComponentTickInterval(CustomTickInterval);	// revert to base ticking
		}
	}

};

#pragma optimize("", on)