

#pragma once

#include "Block.h"
#include "TerminalBlock.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKS_API ATerminalBlock : public ABlock
{
	GENERATED_BODY()
	
public:
	ATerminalBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | TerminalBlock")
		UStaticMeshComponent* TerminalBlockMesh;
	
	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;
};
