﻿#pragma once
#include "Components/ElectricityComponent.h"
#include "Info/BlockInfo.h"
#include "BlockWithElectricity.generated.h"

UINTERFACE(BlueprintType)
class BLOCKS_API UBlockWithElectricity : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class BLOCKS_API IBlockWithElectricity
{
	GENERATED_IINTERFACE_BODY()

public:

	void UpdateHealth(UElectricityComponent* comp, UBlockInfo* info);
};