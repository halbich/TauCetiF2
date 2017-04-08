

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

	void RegisterEntity(UElectricityComponent* comp);
	void UnregisterEntity(UElectricityComponent* comp);
	
};
