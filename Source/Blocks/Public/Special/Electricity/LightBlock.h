#pragma once

#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "LightBlock.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ALightBlock : public ABlock
{
	GENERATED_BODY()

public:
	ALightBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | LightBlock", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* LightBlockMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | LightBlock", meta = (AllowPrivateAccess = "true"))
		UPointLightComponent* Light;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | LightBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;
};
