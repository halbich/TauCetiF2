#pragma once

#include "UObject/NoExportTypes.h"
#include "Commons/Public/Enums.h"
#include "Info/BlockInfo.h"
#include "Definitions/ElectricityComponentDefinition.h"
#include "../ElectricityComponent.h"
#include "Interfaces/ControllerBlock.h"
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

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float EnergyProductionPerSec;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float TotalElectricityAviable;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float MaxElectricityAviable;

	// Percentage of Electricity aviable
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float TotalElectricityAviableFilling;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float EnergyConsumptionPerSec;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float TotalHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float TotalStorableElectricity;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float MaxStorableElectricity;

	// Percentage of Electricity aviable
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float TotalElectricityStorableFilling;

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
		TArray<ABlock*> ControllerBlocks;

	UPROPERTY(Transient)
		TArray<ABlock*> ControllableBlocks;




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

		if (def->IsControlBlock)
		{
			if (def->IsControllable)
			{
				auto c = Cast<ABlock>(comp->GetOwner());
				check(c);
				ControllableBlocks.Add(c);
			}

			if (def->IsController)
			{
				auto c = Cast<ABlock>(comp->GetOwner());
				check(c);
				ControllerBlocks.Add(c);
			}
		}

		auto info = comp->GetBlockInfo();
		ensure(info);

		TotalHealth += info->MaxHealth;
	}

	int32 UnregisterEntity(UElectricityComponent* comp)
	{
		auto r = Entities.Remove(comp);
		EntitiesCount = Entities.Num();

		auto def = comp->GetDefinition();
		ensure(def);

		if (def->IsProducer)
		{
			ElectricityProducers.Remove(comp);
			ProducersCount = ElectricityProducers.Num();
			MaxElectricityAviable -= comp->ElectricityInfo->CurrentObjectMaximumEnergy;
		}

		if (def->IsConsument)
		{
			ElectricityConsumers.Remove(comp);
			ConsumersCount = ElectricityConsumers.Num();
		}

		if (def->IsControlBlock)
		{
			if (def->IsControllable)
			{
				auto c = Cast<ABlock>(comp->GetOwner());
				check(c);
				ControllableBlocks.Remove(c);
			}

			if (def->IsController)
			{
				auto c = Cast<ABlock>(comp->GetOwner());
				check(c);
				ControllerBlocks.Remove(c);
			}
		}

		auto info = comp->GetBlockInfo();
		ensure(info);

		TotalHealth = FMath::Max(0.0f, TotalHealth - info->MaxHealth);		// due to rounding errors, we could get under zero

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
	}

#pragma optimize("", off)

	void CheckControlBlocks()
	{
		TMap<FGuid, TArray<FGuid>> controller_controlledMap;

		for (auto c : ControllerBlocks)
		{
			auto cEl = c->TryGetElectricityComp();
			check(cEl && cEl->Network == this);

			auto icontroller = Cast<IControllerBlock>(c);
			check(icontroller);

			auto controllerRelInfo = c->BlockInfo->RelationsInfo;
			ensure(controllerRelInfo);

			auto arr = &controller_controlledMap.FindOrAdd(controllerRelInfo->ID);

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
				else
					arr->Add(controlledID);
			}

			// and we need to update it after checking so we do not mess with iterated arrays
			ABlock* toUnbind = NULL;
			while (unbind.Dequeue(toUnbind))
				icontroller->Execute_UnbindControl(c, toUnbind);

			TQueue<FGuid> removeRel;

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
					if (!icontroller->Execute_BindControl(c, *controllable))
						removeRel.Enqueue(targetID);
					else
						arr->Add(targetID);
				}
			}

			// and we need to update it after checking so we do not mess with iterated arrays
			FGuid toRemoveGuid;
			while (removeRel.Dequeue(toRemoveGuid))
				controllerRelInfo->RemoveRelationshipsByTargetID(toRemoveGuid);
		}

#if WITH_EDITOR

		for (auto c : ControllableBlocks)
		{
			auto controllableRelInfo = c->BlockInfo->RelationsInfo;
			ensure(controllableRelInfo);
			for (auto cRel : controllableRelInfo->Relationships)
			{
				ensure(cRel->RelationshipType == (uint8)EControlRelationship::IsControlledByTarget);
				auto targetID = cRel->TargetID;

				auto controller = &controller_controlledMap.FindChecked(targetID);

				auto removed = controller->Remove(controllableRelInfo->ID);

				ensure(removed == 1);
			}

		}

		for (auto t : controller_controlledMap)
		{
			ensure(t.Value.Num() == 0);
		}

#endif

	}

#pragma optimize("", on)
};
