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

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FDelegateHandle ListeningHandle;

private:
	void ListeningOnUse(AActor* actor);
};
