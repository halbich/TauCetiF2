

#pragma once
#include "Blocks/Definitions/FBlockMaterialDefinition.h"

/**
 *
 */
class TAUCETIF2_API FBlockDefinition
{

public:
	FBlockDefinition(uint32 id, EShapeType shapeType);
	FBlockDefinition(EBlockName name, EShapeType shapeType);

	uint32 ID;

	EShapeType ShapeType;

	bool AllowPitch;
	bool AllowRoll;
	bool AllowYaw;

	bool HasCustomScaling;
	FVector MeshScale;
	FVector WorldObjectScale;

	FVector MinBlockScale;
	FVector MaxBlockScale;

	TArray<FBlockMaterialDefinition> UsedMaterials;


	void AddMaterials(int8 n, ...);

	FORCEINLINE void SetCustomScale(FVector meshScale, FVector worldObjectScale)
	{
		HasCustomScaling = true;
		MeshScale = meshScale;
		WorldObjectScale = worldObjectScale;
	}
};

