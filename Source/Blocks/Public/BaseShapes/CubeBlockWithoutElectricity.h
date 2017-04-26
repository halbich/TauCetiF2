#pragma once

#include "Block.h"
#include "CubeBlockWithoutElectricity.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ACubeBlockWithoutElectricity : public ABlock
{
	GENERATED_BODY()

public:
	ACubeBlockWithoutElectricity();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CubeBlock")
		UStaticMeshComponent* CubeMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | TerminalBlock")
		UStaticMeshComponent* CubeMeshOutline;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;
};
