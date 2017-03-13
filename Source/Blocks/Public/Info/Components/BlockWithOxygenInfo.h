#pragma once

#include "UObject/NoExportTypes.h"
#include "BlockWithOxygenInfo.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API UBlockWithOxygenInfo : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
		float CurrentFillingValue;
};
