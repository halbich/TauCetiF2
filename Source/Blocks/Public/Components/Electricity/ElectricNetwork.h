#pragma once

#include "UObject/NoExportTypes.h"
#include "Commons/Public/Enums.h"
#include "Info/BlockInfo.h"
#include "Definitions/ElectricityComponentDefinition.h"
#include "../ElectricityComponent.h"
#include "Interfaces/ControllerBlock.h"
#include "Interfaces/ControllableBlock.h"
#include "ElectricNetwork.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class BLOCKS_API UElectricNetwork : public UObject
{
	GENERATED_BODY()

public:
	UElectricNetwork();

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 EntitiesCount;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 ProducersCount;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 ConsumersCount;



	// updated in updateStatistics()
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float EnergyProductionPerSec;

	// updated in tick()
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float TotalElectricityAviable;

	// updated in network maintenance
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float MaxElectricityAviable;

	// Percentage of Electricity aviable
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float TotalElectricityAviableFilling;


	// updated in updateStatistics()
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float EnergyConsumptionPerSec;

	// updated in tick()
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float TotalElectricityRequired;

	/*UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float MaxStorableElectricity;*/

	// Percentage of +/-, updated in tick
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float TotalElectricityPlusMinus;



	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float TotalHealth;



	UPROPERTY(Transient)
		TArray<UElectricityComponent*> Entities;

	TQueue<UElectricityComponent*> ToRecompute;

	UPROPERTY(Transient)
		EElectricNetworkState NetworkState;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ToRepairEntities;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ImportantRepairEntities;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> CriticalRepairEntities;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ElectricityProducers;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ElectricityConsumers;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ElectricityStorages;

	UPROPERTY(Transient)
		TArray<ABlock*> ControllerBlocks;

	UPROPERTY(Transient)
		TArray<ABlock*> ControllableBlocks;

	UPROPERTY(Transient)
		TArray<ABlock*> PatternBlocks;

	UPROPERTY(Transient)
		bool NetworkChecked;
private:

	FORCEINLINE TArray<UElectricityComponent*>& getArrayBySeverity(UElectricityComponent* comp)
	{
		//switch (comp->HealthSeverity)
		//{
		//case EHealthSeverity::OK: {
		//	//return false;
		//	break;
		//}
		//case EHealthSeverity::ToRepair: {
		return ToRepairEntities;
		//	break;
		//}
		//default: {
		//	checkNoEntry();
		//	break;
		//}

		//}

	//	return NULL;
	}

public:

	FORCEINLINE void ForceInvalidateNetwork()
	{
		NetworkState = EElectricNetworkState::Invalid;
		for (auto netEnt : Entities)
			netEnt->ComponentNetworkState = EElectricNetworkState::Invalid;

		ToRecompute.Empty();
	}

	void RegisterEntity(UElectricityComponent* comp)
	{
		Entities.Add(comp);
		EntitiesCount = Entities.Num();

		auto def = comp->GetDefinition();
		ensure(def);

		if (def->IsProducer)
		{
			ElectricityProducers.Add(comp);
			ProducersCount = ElectricityProducers.Num();
			MaxElectricityAviable += comp->ElectricityInfo->CurrentObjectMaximumEnergy;
		}

		if (def->IsConsument)
		{
			ElectricityConsumers.Add(comp);
			ConsumersCount = ElectricityConsumers.Num();
		}

		if (def->IsProducer && def->IsConsument)
		{
			ElectricityStorages.Add(comp);
		}

		auto c = Cast<ABlock>(comp->GetOwner());
		check(c);

		if (def->IsControlBlock)
		{
			if (def->IsControllable)
				ControllableBlocks.Add(c);

			if (def->IsController)
				ControllerBlocks.Add(c);
		}

		auto info = comp->GetBlockInfo();
		ensure(info);

		TotalHealth += info->MaxHealth;

		if (c->Definition.GetDefaultObject()->UsingInPatterns)
			auto rem = PatternBlocks.Add(c);

		NetworkChecked = false;
	}

	int32 UnregisterEntity(UElectricityComponent* comp)
	{
		auto r = Entities.Remove(comp);
		ensure(r > 0);

		EntitiesCount = Entities.Num();

		auto def = comp->GetDefinition();
		ensure(def);

		if (def->IsProducer)
		{
			auto rem = ElectricityProducers.Remove(comp);
			ensure(rem > 0);
			ProducersCount = ElectricityProducers.Num();
			MaxElectricityAviable -= comp->ElectricityInfo->CurrentObjectMaximumEnergy;
		}

		if (def->IsConsument)
		{
			auto rem = ElectricityConsumers.Remove(comp);
			ensure(rem > 0);
			ConsumersCount = ElectricityConsumers.Num();
		}

		if (def->IsProducer && def->IsConsument)
		{
			auto rem = ElectricityStorages.Remove(comp);
			ensure(rem > 0);
		}

		auto c = Cast<ABlock>(comp->GetOwner());
		check(c);

		if (def->IsControlBlock)
		{
			if (def->IsControllable)
			{
				auto rem = ControllableBlocks.Remove(c);
				ensure(rem > 0);
			}

			if (def->IsController)
			{
				auto rem = ControllerBlocks.Remove(c);
				ensure(rem > 0);
			}
		}

		auto info = comp->GetBlockInfo();
		ensure(info);

		TotalHealth = FMath::Max(0.0f, TotalHealth - info->MaxHealth);		// due to rounding errors, we could get under zero

		if (c->Definition.GetDefaultObject()->UsingInPatterns)
		{
			auto rem = PatternBlocks.Remove(c);
			ensure(rem > 0);
		}

		NetworkChecked = false;

		return r;
	}

	FORCEINLINE void RegisterEntitySeverity(UElectricityComponent* comp, EHealthSeverity newSeverity)
	{
	}

	FORCEINLINE void EmptyNetwork()
	{
		Entities.Empty();
		ToRepairEntities.Empty();
		ImportantRepairEntities.Empty();
		CriticalRepairEntities.Empty();
		ElectricityProducers.Empty();
		ElectricityConsumers.Empty();
		ControllerBlocks.Empty();
		ControllableBlocks.Empty();
		NetworkChecked = false;
	}

#pragma optimize("", off)

	void CheckControlBlocks()
	{
		if (NetworkChecked)
			return;

		for (auto c : ControllerBlocks)
		{
			auto cEl = c->TryGetElectricityComp();
			check(cEl && cEl->Network == this);

			auto icontroller = Cast<IControllerBlock>(c);
			check(icontroller);

			auto controllerRelInfo = c->BlockInfo->RelationsInfo;
			ensure(controllerRelInfo);

			auto controlled = icontroller->Execute_GetControlledBlocks(c);

			TQueue<ABlock*> unbind;

			// we need to check currently binded. Network could change so out items could be inaccessible
			for (auto con : controlled)
			{
				auto conEl = con->TryGetElectricityComp();
				check(conEl);

				auto controlledID = con->BlockInfo->RelationsInfo->ID;

				auto hasRel = controllerRelInfo->Relationships.FindByPredicate([controlledID](URelationshipInfo* info) {
					return info->TargetID == controlledID && info->RelationshipType == (uint8)EControlRelationship::IsControllingTarget;
				});

				if (conEl->Network != this || !hasRel)		// our controlled item's network has changed, we need to unbind this
					unbind.Enqueue(con);
			}

			// and we need to update it after checking so we do not mess with iterated arrays
			ABlock* toUnbind = NULL;
			while (unbind.Dequeue(toUnbind))
			{
				auto _u = icontroller->Execute_UnbindControl(c, toUnbind);
				ensure(_u);
			}

			TQueue<FGuid> removeRel;
			TQueue<ABlock*> bind;

			// now we need to bind items, this is usually after loading level
			for (auto cRel : controllerRelInfo->Relationships)
			{
				ensure(cRel->RelationshipType == (uint8)EControlRelationship::IsControllingTarget);
				auto targetID = cRel->TargetID;

				auto controllable = ControllableBlocks.FindByPredicate([targetID](ABlock* block) {
					return block->BlockInfo->RelationsInfo->ID == targetID;
				});

				if (!controllable)
					removeRel.Enqueue(targetID);
				else
				{
					bind.Enqueue(*controllable);
				}
			}

			ABlock* toBind = NULL;
			while (bind.Dequeue(toBind))
			{
				auto currentBindedC = Cast<IControllableBlock>(toBind)->Execute_GetController(toBind);

				if (c == currentBindedC)
					continue;

				auto _b = icontroller->Execute_BindControl(c, toBind);
				ensure(_b);
			}

			// and we need to update it after checking so we do not mess with iterated arrays
			FGuid toRemoveGuid;
			while (removeRel.Dequeue(toRemoveGuid))
				controllerRelInfo->RemoveRelationshipsByTargetID(toRemoveGuid);
		}

		NetworkChecked = true;
	}

#pragma optimize("", on)
};
