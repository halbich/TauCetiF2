#pragma once

#include "CubeBlockWithoutElectricity.h"
#include "Components/ElectricityComponent.h"
#include "CubeBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ACubeBlock : public ACubeBlockWithoutElectricity
{
	GENERATED_BODY()

public:
	ACubeBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CubeBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

};
