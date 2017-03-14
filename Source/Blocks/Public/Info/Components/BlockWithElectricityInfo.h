#pragma once

#include "UObject/NoExportTypes.h"
#include "BlockWithElectricityInfo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class BLOCKS_API UBlockWithElectricityInfo : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockWithElectricityInfo")
		float CurrentObjectEnergy;
};