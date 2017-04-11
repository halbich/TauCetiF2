

#pragma once

#include "UObject/NoExportTypes.h"
//#include "../ElectricityComponent.h"
#include "Commons/Public/Enums.h"
#include "ElectricNetwork.generated.h"

class UElectricityComponent;

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

	
	FORCEINLINE void RegisterEntity(UElectricityComponent* comp, bool isProducer, bool isConsumer)
	{
		Entities.Add(comp);
		EntitiesCount = Entities.Num();

		if (isProducer)
		{
			ElectricityProducers.Add(comp);
			ProducersCount = ElectricityProducers.Num();
		}

		if (isConsumer)
		{
			ElectricityConsumers.Add(comp);
			ConsumersCount = ElectricityConsumers.Num();
		}
	}


	FORCEINLINE int32 UnregisterEntity(UElectricityComponent* comp)
	{
		auto r = Entities.Remove(comp);
		EntitiesCount = Entities.Num();
		return r;
	}

	FORCEINLINE void RegisterEntitySeverity(UElectricityComponent* comp, EHealthSeverity newSeverity)
	{

	}
	
};
