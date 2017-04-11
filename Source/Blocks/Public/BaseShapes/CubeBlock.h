#pragma once

#include "CubeBlockWithoutElectricity.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "CubeBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ACubeBlock : public ACubeBlockWithoutElectricity, public IBlockWithElectricity
{
	GENERATED_BODY()

public:
	ACubeBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CubeBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}
};
