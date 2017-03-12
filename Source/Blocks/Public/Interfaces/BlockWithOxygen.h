

#pragma once

#include "Components/OxygenComponent.h"

#include "BlockWithOxygen.generated.h"

UINTERFACE(BlueprintType)
class BLOCKS_API UBlockWithOxygen : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class BLOCKS_API IBlockWithOxygen
{
	GENERATED_IINTERFACE_BODY()

public:

	void SetupComponentData();

};