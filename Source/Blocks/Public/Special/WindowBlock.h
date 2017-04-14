#pragma once

#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "WindowBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API AWindowBlock : public ABlock, public IBlockWithElectricity
{
	GENERATED_BODY()

public:
	AWindowBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | WindowBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}
};
