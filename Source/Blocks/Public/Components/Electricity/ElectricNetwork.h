

#pragma once

#include "UObject/NoExportTypes.h"
//#include "../ElectricityComponent.h"
#include "ElectricNetwork.generated.h"

class UElectricityComponent;

/**
 * 
 */
UCLASS(BlueprintType)
class BLOCKS_API UElectricNetwork : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Transient)
		TArray<UElectricityComponent*> entities;

	void RegisterEntity(UElectricityComponent* comp);
	void UnregisterEntity(UElectricityComponent* comp);
	
};
