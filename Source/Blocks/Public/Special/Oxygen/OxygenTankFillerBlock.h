

#pragma once

#include "Block.h"
#include "OxygenTankFillerBlock.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKS_API AOxygenTankFillerBlock : public ABlock
{
	GENERATED_BODY()
	
public:

	AOxygenTankFillerBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller")
		UStaticMeshComponent* OxygenTankFillerBodyMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller")
		UStaticMeshComponent* OxygenTankFillerHeadMesh;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	
	
	
};
