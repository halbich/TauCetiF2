

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

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> Entities;

	TQueue<UElectricityComponent*> ToRecompute;

	UPROPERTY(Transient)
		EElectricNetworkState NetworkState;

	
	FORCEINLINE void RegisterEntity(UElectricityComponent* comp)
	{
		Entities.Add(comp);
		EntitiesCount = Entities.Num();
	}


	FORCEINLINE int32 UnregisterEntity(UElectricityComponent* comp)
	{
		auto r = Entities.Remove(comp);
		EntitiesCount = Entities.Num();
		return r;
	}


	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | Electric Network")
		int32 EntitiesCount;
};
