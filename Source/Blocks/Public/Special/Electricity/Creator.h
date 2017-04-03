

#pragma once

#include "Block.h"
#include "Creator.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API ACreator : public ABlock
{
	GENERATED_BODY()

public:
	ACreator();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CreatorBlock")
		UStaticMeshComponent* CreatorMesh;


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CreatorBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	//virtual void OnConstruction(const FTransform& Transform) override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

};
