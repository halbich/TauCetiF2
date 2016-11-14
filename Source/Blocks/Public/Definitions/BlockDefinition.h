

#pragma once

#include "Object.h"
#include "Commons/Public/Enums.h"
#include "BlockMeshStructureDefinition.h"
#include "BlockDefinition.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLOCKS_API UBlockDefinition : public UObject
{
	GENERATED_BODY()
	
public:
		UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition")
		EShapeType ShapeType;
	
		UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition")
			TArray<FBlockMeshStructureDefinition> MeshStructure;
};
