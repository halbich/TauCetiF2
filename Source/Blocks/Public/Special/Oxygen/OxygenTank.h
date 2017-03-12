#pragma once

#include "Block.h"
#include "PickableBlock.h"
#include "OxygenTank.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API AOxygenTank : public ABlock, public IPickableBlock
{
	GENERATED_BODY()

public:

	AOxygenTank();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTank")
		UStaticMeshComponent* OxygenTankMesh;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FDelegateHandle ListeningHandle;

private:
	void ListeningOnUse(AActor* actor);
};
