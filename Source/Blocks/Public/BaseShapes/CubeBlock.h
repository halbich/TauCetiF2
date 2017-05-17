#pragma once

#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "CubeBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ACubeBlock : public ABlock, public IBlockWithElectricity
{
	GENERATED_BODY()

public:
	ACubeBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CubeBlock")
		UStaticMeshComponent* CubeMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CubeBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;
};
