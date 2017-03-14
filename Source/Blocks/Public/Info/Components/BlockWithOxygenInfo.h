#pragma once

#include "UObject/NoExportTypes.h"
#include "BlockWithOxygenInfo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class BLOCKS_API UBlockWithOxygenInfo : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockWithOxygenInfo")
		float CurrentFillingValue;
};
