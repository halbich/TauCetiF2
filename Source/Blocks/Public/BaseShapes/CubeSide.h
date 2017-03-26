#pragma once

#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "CubeSide.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ACubeSide : public ABlock
{
	GENERATED_BODY()

public:
	ACubeSide();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CubeSide")
		UStaticMeshComponent* CubeSideMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CubeSide", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;
};
