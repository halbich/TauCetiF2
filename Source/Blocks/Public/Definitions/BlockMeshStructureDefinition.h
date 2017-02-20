#pragma once

#include "BlockMaterialDefinition.h"
#include "BlockMeshStructureDefinition.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FBlockMeshStructureDefinition
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block")
		UStaticMesh* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block", meta = (ShowOnlyInnerProperties))
		TArray<FBlockMaterialDefinition> Materials;

	FBlockMeshStructureDefinition()
	{
	}
};
