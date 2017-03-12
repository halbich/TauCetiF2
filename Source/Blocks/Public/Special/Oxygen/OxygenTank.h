#pragma once

#include "Block.h"
#include "PickableBlock.h"
#include "Components/OxygenComponent.h"
#include "BlockWithOxygen.h"
#include "OxygenTank.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API AOxygenTank : public ABlock, public IPickableBlock, public IBlockWithOxygen
{
	GENERATED_BODY()

public:

	AOxygenTank();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTank")
		UStaticMeshComponent* OxygenTankMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTank", meta = (AllowPrivateAcces = "true"))
		UOxygenComponent* OxygenComponent;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FDelegateHandle ListeningHandle;

private:
	void ListeningOnUse(AActor* actor);
};
