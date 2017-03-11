

#pragma once

#include "Block.h"
#include "OxygenTank.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKS_API AOxygenTank : public ABlock
{
	GENERATED_BODY()
	
public:

	AOxygenTank();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTank")
		UStaticMeshComponent* OxygenTankMesh;
	
	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

};
