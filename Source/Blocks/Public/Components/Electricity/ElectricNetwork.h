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
		for (auto c : ControllerBlocks)
		{
			auto cEl = c->TryGetElectricityComp();
			check(cEl && cEl->Network == this);

			auto icontroller = Cast<IControllerBlock>(c);
			check(icontroller);

			auto controlled = icontroller->Execute_GetControlledBlocks(c);

			TQueue<ABlock*> unbind;

			for (auto con : controlled)
			{
				auto conEl = con->TryGetElectricityComp();
				check(conEl);

				if (conEl->Network != this)		// our contrlolled item's network has changed, we need to unbind this
					unbind.Enqueue(con);
			}

			ABlock* toUnbind = NULL;
			while (unbind.Dequeue(toUnbind))
				icontroller->Execute_UnbindControl(c, toUnbind);

		}

	}

#pragma optimize("", on)
};
