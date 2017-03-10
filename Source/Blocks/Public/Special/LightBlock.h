#pragma once

#include "Block.h"
#include "LightBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ALightBlock : public ABlock
{
	GENERATED_BODY()

public:
	ALightBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | LightBlock")
		UStaticMeshComponent* LightBlockMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | LightBlock")
		UPointLightComponent* Light;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;
};
