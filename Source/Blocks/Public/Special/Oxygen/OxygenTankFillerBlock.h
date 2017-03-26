#pragma once

#include "Block.h"
#include "Components/OxygenComponent.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithOxygen.h"
#include "OxygenTankFillerBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API AOxygenTankFillerBlock : public ABlock, public IBlockWithOxygen
{
	GENERATED_BODY()

public:

	AOxygenTankFillerBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller")
		UStaticMeshComponent* OxygenTankFillerBodyMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller")
		UStaticMeshComponent* OxygenTankFillerHeadMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller", meta = (AllowPrivateAcces = "true"))
		UOxygenComponent* OxygenComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | OxygenTankFiller", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;
};
