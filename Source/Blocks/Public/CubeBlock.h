#pragma once

#include "Block.h"
#include "CubeBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ACubeBlock : public ABlock
{
	GENERATED_BODY()

public:
	ACubeBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CubeBlock")
		UStaticMeshComponent* CubeMesh;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;
};
