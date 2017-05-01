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

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | GameElectricityComponent")
		bool TickingEnabled;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddToWorldNetwork(UElectricityComponent* comp);

	void RemoveFromWorldNetwork(UElectricityComponent* comp);

private:

	static const double maxFloatingTime;
	FCriticalSection SectionLock;

	FORCEINLINE void enqueueItem(UElectricityComponent* comp) {
		comp->ComponentNetworkState = EElectricNetworkState::InRecompute;
		comp->Network->ToRecompute.Enqueue(comp);
		comp->Network->NetworkState = EElectricNetworkState::InRecompute;
	}

	FORCEINLINE UElectricNetwork* addToNetwork(UElectricityComponent* comp, UElectricNetwork* network)
	{
		ensure(comp->Network != network);

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

		auto rem = networks.Remove(smaller);
		ensure(rem > 0);
		smaller->MarkPendingKill();
	}

	FORCEINLINE void healGroup(UElectricNetwork* n, float& maxAviable, const float maxAviableLimit, TArray<UElectricityComponent*> & healItems, TQueue<UElectricityComponent*>& checkStatus)
	{
		auto inRec = n->NetworkState == EElectricNetworkState::InRecompute;

		auto p = n->ElectricityProducers.Num();
		auto pc = p + n->ElectricityStorages.Num();

		auto cc = healItems.Num();
		if (cc > 0 && pc > 0 && maxAviable > 0)
		{
			for (int32 i = 0; i < cc; i++)
			{
				if (FMath::IsNearlyZero(maxAviable))
					return;

				auto ind = FMath::RandHelper(cc);	ensure(healItems.IsValidIndex(ind));

				auto consElem = healItems[ind];
				if (inRec && consElem->ComponentNetworkState != EElectricNetworkState::Valid)
					continue;

				auto bi = consElem->GetBlockInfo();
				auto missingHealthEnergy = FMath::Max(0.0f, bi->MaxHealth - bi->Health) * GameDefinitions::HealthToEnergy;

				// TODO take only percentage

				auto missingPercentage = FMath::Min(missingHealthEnergy, maxAviable);

				if (FMath::IsNearlyZero(missingPercentage))
					continue;

				auto producerSelect = FMath::RandHelper(pc);

				UElectricityComponent* elemProducer;
				if (producerSelect < p)
				{
					// we are in producers part
					ensure(n->ElectricityProducers.IsValidIndex(producerSelect));

					elemProducer = n->ElectricityProducers[producerSelect];
					if (inRec && elemProducer->ComponentNetworkState != EElectricNetworkState::Valid)
						continue;
				}
				else
				{
					producerSelect -= p;

					// we are in storage part
					ensure(n->ElectricityStorages.IsValidIndex(producerSelect));

					elemProducer = n->ElectricityStorages[producerSelect];
					if (inRec && elemProducer->ComponentNetworkState != EElectricNetworkState::Valid)
						continue;
				}

				float actuallyPutted = 0;
				float actuallyObtained = 0;
				float actuallyReturned = 0;
				if (elemProducer->ObtainAmount(missingPercentage, actuallyObtained))
				{
					auto healed = actuallyObtained*GameDefinitions::EnergyToHealth;

					bi->Health = FMath::Clamp(healed + bi->Health, 0.0f, bi->MaxHealth);
					bi->HealthDamageHealed += healed;

					maxAviable -= actuallyObtained;

					checkStatus.Enqueue(consElem);
				}
			}
		}
	}

	FORCEINLINE void doHealing(UElectricNetwork* n, float& maxAviable)
	{
		float totalElectricityAviable = maxAviable;

		TQueue<UElectricityComponent*> checkStatus;
		float criticalAviable = 0.5f * totalElectricityAviable;
		float importantAviable = 0.5f * criticalAviable;
		float repairAviable = importantAviable;

		// TODO
		float criticalHealth = 0.0f;
		for (auto critical : n->CriticalRepairEntities)
		{
			if (critical->ComponentNetworkState == EElectricNetworkState::Valid)
				criticalHealth += critical->GetBlockInfo()->Health;
		}

		n->CriticalRepairHealth = criticalHealth;
		n->CriticalRepairHealthPercentage = FMath::IsNearlyZero(n->CriticalRepairMaxHealth) ? 0 : criticalHealth / n->CriticalRepairMaxHealth;
		healGroup(n, maxAviable, criticalAviable, n->CriticalRepairEntities, checkStatus);

		float importantHealth = 0.0f;
		for (auto important : n->ImportantRepairEntities)
		{
			if (important->ComponentNetworkState == EElectricNetworkState::Valid)
				importantHealth += important->GetBlockInfo()->Health;
		}

		n->ImportantRepairHealth = importantHealth;
		n->ImportantRepairHealthPercentage = FMath::IsNearlyZero(n->ImportantRepairMaxHealth) ? 0 : importantHealth / n->ImportantRepairMaxHealth;
		healGroup(n, maxAviable, importantAviable, n->ImportantRepairEntities, checkStatus);

		float repairHealth = 0.0f;
		for (auto toRepair : n->ToRepairEntities)
		{
			if (toRepair->ComponentNetworkState == EElectricNetworkState::Valid)
				repairHealth += toRepair->GetBlockInfo()->Health;
		}

		n->ToRepairHealth = repairHealth;
		n->ToRepairHealthPercentage = FMath::IsNearlyZero(n->ToRepairMaxHealth) ? 0 : repairHealth / n->ToRepairMaxHealth;
		healGroup(n, maxAviable, repairAviable, n->ToRepairEntities, checkStatus);

		UElectricityComponent* c;
		while (checkStatus.Dequeue(c))
		{
			auto bi = c->GetBlockInfo();
			auto lastSev = bi->HealthSeverity;
			if (!bi->UpdateHealthSeverity())
				continue;	// severity has not changed

			n->RefreshHealthSeverity(c, lastSev);
		}
	}

	FORCEINLINE void tickUpdateNetwork(UElectricNetwork* n, float deltaTime)
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
				totalHealth += ent->GetBlockInfo()->Health;

		n->NetworkHealth = totalHealth;
		n->NetworkHealthPercentage = FMath::IsNearlyZero(n->NetworkMaxHealth) ? 0 : totalHealth / n->NetworkMaxHealth;

		auto maxAviable = producersEnergyAviable + storagesEnergyAviable;

		if (maxAviable > 0)
			doHealing(n, maxAviable);

		auto inRec = n->NetworkState == EElectricNetworkState::InRecompute;

		auto p = n->ElectricityProducers.Num();
		auto pc = p + n->ElectricityStorages.Num();

		auto cc = n->ElectricityConsumers.Num();
		if (cc > 0 && pc > 0 && maxAviable > 0)
		{
			for (int32 i = 0; i < cc; i++)
			{
				auto ind = FMath::RandHelper(cc);	ensure(n->ElectricityConsumers.IsValidIndex(ind));

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


		float damageInfo = 0.0f;
		float healedInfo = 0.0f;
		for (auto block : n->Entities)
		{
			auto bi = block->GetBlockInfo();
			damageInfo += bi->HealthDamageTaken;
			bi->HealthDamageTaken = 0;
			healedInfo += bi->HealthDamageHealed;
			bi->HealthDamageHealed = 0;
		}

		n->HealthDamageTaken = damageInfo;
		n->HealthDamageHealed = healedInfo;
	}

	FORCEINLINE void processNetwork(UElectricNetwork* network)
	{
		UElectricityComponent* part;
		auto deq = network->ToRecompute.Dequeue(part);

		if (!deq || !part || !part->IsValidLowLevel() || part->IsPendingKill() || part->Network != network)
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
			for (auto n : networks)
			{
				if (networksTodelete.Contains(n))
					continue;

				if (n->Entities.Num() == 0)
				{
					networksTodelete.AddUnique(n);
					continue;
				}

				n->CheckControlBlocks();
			}

			for (auto toDel : networksTodelete)
			{
				auto rem = networks.Remove(toDel);
				ensure(rem > 0);
				ensure(toDel->Entities.Num() == 0);
				ensure(toDel->ToRecompute.IsEmpty());

				toDel->MarkPendingKill();
			}
			networksTodelete.Empty();
		}
	}
};

