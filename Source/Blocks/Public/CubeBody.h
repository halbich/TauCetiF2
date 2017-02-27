#pragma once

#include "Block.h"
#include "CubeBody.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ACubeBody : public ABlock
{
	GENERATED_BODY()

public:
	ACubeBody();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CubeBody")
		UStaticMeshComponent* CubeBodyMesh;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;
};
