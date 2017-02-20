#pragma once

#include "Block.h"
#include "DoorBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ADoorBlock : public ABlock
{
	GENERATED_BODY()

public:
	ADoorBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | DoorBlock")
		UStaticMeshComponent* DoorBlockMesh;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;
};
