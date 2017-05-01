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

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> Entities;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 EntitiesCount;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ElectricityProducers;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 ProducersCount;

	// updated in updateStatistics()
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float EnergyProductionPerSec;

	// updated in tick()
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ProducersEnergyAviable;

	// updated in network maintenance
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ProducersEnergyMaxAviable;

	// Percentage of Electricity aviable
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ProducersEnergyAviableFilling;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ElectricityConsumers;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 ConsumersCount;

	// updated in updateStatistics()
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float EnergyConsumptionPerSec;

	// updated in tick()
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ConsumersEnergyAviable;

	// updated in network maintenance
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ConsumersEnergyMaxAviable;

	// Percentage of Electricity aviable
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ConsumersEnergyAviableFilling;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ElectricityStorages;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 StoragesCount;

	// updated in tick()
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float StoragesEnergyAviable;

	// updated in network maintenance
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float StoragesEnergyMaxAviable;

	// Percentage of Electricity aviable
	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float StoragesEnergyAviableFilling;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float NetworkMaxHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float NetworkHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float NetworkHealthPercentage;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float HealthDamageTaken;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float HealthDamageHealed;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 ToRepairHealthCount;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ToRepairEntities;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ToRepairMaxHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ToRepairHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ToRepairHealthPercentage;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 ImportantRepairHealthCount;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ImportantRepairEntities;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ImportantRepairMaxHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ImportantRepairHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float ImportantRepairHealthPercentage;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 CriticalRepairHealthCount;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> CriticalRepairEntities;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float CriticalRepairMaxHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float CriticalRepairHealth;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		float CriticalRepairHealthPercentage;

	TQueue<UElectricityComponent*> ToRecompute;

	UPROPERTY(Transient)
		EElectricNetworkState NetworkState;

	UPROPERTY(Transient)
		TArray<ABlock*> ControllerBlocks;

	UPROPERTY(Transient)
		TArray<ABlock*> ControllableBlocks;

	UPROPERTY(Transient)
		TArray<ABlock*> PatternBlocks;

	UPROPERTY(Transient)
		bool NetworkChecked;
private:

	/*FORCEINLINE*/ void tryUpdateArrayBySeverity(UElectricityComponent* comp)
	{
		auto b = comp->GetBlockInfo();
		switch (b->HealthSeverity)
		{
		case EHealthSeverity::ToRepair: {
			ToRepairEntities.Add(comp);
			ToRepairHealthCount = ToRepairEntities.Num();
			ToRepairMaxHealth += b->MaxHealth;
			break;
		}
		case EHealthSeverity::Important: {
			ImportantRepairEntities.Add(comp);
			ImportantRepairHealthCount = ImportantRepairEntities.Num();
			ImportantRepairMaxHealth += b->MaxHealth;
			break;
		}
		case EHealthSeverity::Critical: {
			CriticalRepairEntities.Add(comp);
			CriticalRepairHealthCount = CriticalRepairEntities.Num();
			CriticalRepairMaxHealth += b->MaxHealth;
			break;
		}
		}
	}

	/*FORCEINLINE*/ void tryUpdateArrayBySeverityRem(UElectricityComponent* comp, EHealthSeverity oldSeverity)
	{
		auto b = comp->GetBlockInfo();
		switch (oldSeverity)
		{
		case EHealthSeverity::ToRepair: {
			auto rem = ToRepairEntities.Remove(comp);
			ensure(rem > 0);
			ToRepairHealthCount = ToRepairEntities.Num();
			ToRepairMaxHealth = FMath::Max(0.0f, ToRepairMaxHealth - b->MaxHealth);
			break;
		}
		case EHealthSeverity::Important: {
			auto rem = ImportantRepairEntities.Remove(comp);
			ensure(rem > 0);
			ImportantRepairHealthCount = ImportantRepairEntities.Num();
			ImportantRepairMaxHealth = FMath::Max(0.0f, ImportantRepairMaxHealth - b->MaxHealth);
			break;
		}
		case EHealthSeverity::Critical: {
			auto rem = CriticalRepairEntities.Remove(comp);
			ensure(rem > 0);
			CriticalRepairHealthCount = CriticalRepairEntities.Num();
			CriticalRepairMaxHealth = FMath::Max(0.0f, CriticalRepairMaxHealth - b->MaxHealth);
			break;
		}
		}
	}

public:

	FORCEINLINE void ForceInvalidateNetwork()
	{
		NetworkState = EElectricNetworkState::Invalid;
		for (auto netEnt : Entities)
		{
			netEnt->ComponentNetworkState = EElectricNetworkState::Invalid;

			UE_LOG(LogTemp, Log, TEXT("Invalidationg for part: %s in network %s"), *netEnt->GetName(), *netEnt->Network->GetName());
		}
		ToRecompute.Empty();
	}

	void RegisterEntity(UElectricityComponent* comp)
	{
		Entities.Add(comp);
		EntitiesCount = Entities.Num();

		auto def = comp->GetDefinition();
		ensure(def);

		if (def->IsProducer && !def->IsConsument)
		{
			ElectricityProducers.Add(comp);
			ProducersCount = ElectricityProducers.Num();
			ProducersEnergyMaxAviable += comp->ElectricityInfo->CurrentObjectMaximumEnergy;
		}

		if (def->IsConsument && !def->IsProducer)
		{
			ElectricityConsumers.Add(comp);
			ConsumersCount = ElectricityConsumers.Num();
			ConsumersEnergyMaxAviable += comp->ElectricityInfo->CurrentObjectMaximumEnergy;
		}

		if (def->IsProducer && def->IsConsument)
		{
			ElectricityStorages.Add(comp);
			StoragesCount = ElectricityStorages.Num();
			StoragesEnergyMaxAviable += comp->ElectricityInfo->CurrentObjectMaximumEnergy;
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

		NetworkMaxHealth += info->MaxHealth;

		tryUpdateArrayBySeverity(comp);

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

		if (def->IsProducer && !def->IsConsument)
		{
			auto rem = ElectricityProducers.Remove(comp);
			ensure(rem > 0);
			ProducersCount = ElectricityProducers.Num();
			ProducersEnergyMaxAviable = FMath::Max(0.0f, ProducersEnergyMaxAviable - comp->ElectricityInfo->CurrentObjectMaximumEnergy);
		}

		if (def->IsConsument && !def->IsProducer)
		{
			auto rem = ElectricityConsumers.Remove(comp);
			ensure(rem > 0);
			ConsumersCount = ElectricityConsumers.Num();
			ConsumersEnergyMaxAviable = FMath::Max(0.0f, ConsumersEnergyMaxAviable - comp->ElectricityInfo->CurrentObjectMaximumEnergy);
		}

		if (def->IsProducer && def->IsConsument)
		{
			auto rem = ElectricityStorages.Remove(comp);
			ensure(rem > 0);
			StoragesCount = ElectricityStorages.Num();
			StoragesEnergyMaxAviable = FMath::Max(0.0f, StoragesEnergyMaxAviable - comp->ElectricityInfo->CurrentObjectMaximumEnergy);
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

		NetworkMaxHealth = FMath::Max(0.0f, NetworkMaxHealth - info->MaxHealth);		// due to rounding errors, we could get under zero

		tryUpdateArrayBySeverityRem(comp, comp->GetBlockInfo()->HealthSeverity);

		if (c->Definition.GetDefaultObject()->UsingInPatterns)
		{
			auto rem = PatternBlocks.Remove(c);
			ensure(rem > 0);
		}

		NetworkChecked = false;

		return r;
	}

	FORCEINLINE void EmptyNetwork()
	{
		Entities.Empty();

		ToRepairEntities.Empty();
		ImportantRepairEntities.Empty();
		CriticalRepairEntities.Empty();

		ElectricityProducers.Empty();
		ElectricityConsumers.Empty();
		ElectricityStorages.Empty();

		ControllerBlocks.Empty();
		ControllableBlocks.Empty();
		PatternBlocks.Empty();

		NetworkChecked = false;
	}

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

	FORCEINLINE void RefreshHealthSeverity(UElectricityComponent* comp, EHealthSeverity oldSeverity)
	{
		tryUpdateArrayBySeverityRem(comp, oldSeverity);
		tryUpdateArrayBySeverity(comp);
	}
};
