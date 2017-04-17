#pragma once

#include "UObject/NoExportTypes.h"
#include "Commons/Public/Enums.h"
#include "Info/BlockInfo.h"
#include "Definitions/ElectricityComponentDefinition.h"
#include "../ElectricityComponent.h"
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
		TArray<UElectricityComponent*> ControllerBlocks;

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> ControllableBlocks;




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
				ControllableBlocks.Add(comp);
			}

			if (def->IsController)
			{
				ControllerBlocks.Add(comp);
			}
		}

		auto info = comp->GetBlockInfo();
		ensure(info);

		TotalHealth += info->MaxHealth;
	}

	FORCEINLINE int32 UnregisterEntity(UElectricityComponent* comp)
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
				ControllableBlocks.Remove(comp);
			}

			if (def->IsController)
			{
				ControllerBlocks.Remove(comp);
			}
		}

		auto info = comp->GetBlockInfo();
		ensure(info);

		TotalHealth -= info->MaxHealth;
		check(TotalHealth >= 0);

		return r;
	}

	FORCEINLINE void RegisterEntitySeverity(UElectricityComponent* comp, EHealthSeverity newSeverity)
	{
	}
};
