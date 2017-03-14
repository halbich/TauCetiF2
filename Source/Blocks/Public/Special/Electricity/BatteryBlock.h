#pragma once

#include "Block.h"
#include "BatteryBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ABatteryBlock : public ABlock
{
	GENERATED_BODY()

public:

	ABatteryBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | BatteryBlock")
		UStaticMeshComponent* BatteryBlockMesh;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;
};
