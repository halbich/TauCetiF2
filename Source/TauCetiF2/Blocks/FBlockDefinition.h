

#pragma once

#include "Helpers/Helpers.h"

class TAUCETIF2_API FBlockMaterialDefinition
{
public:

	FVector ApplyCoordinates;
	EMaterialInstance MaterialInstance;

	FBlockMaterialDefinition(const EMaterialInstance instance, bool useX, bool useY, bool useZ) : 
		MaterialInstance(instance),
		ApplyCoordinates(useX ? 1 : 0, useY ? 1 : 0, useZ ? 1 : 0)
	{
	};

};

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

	TArray<FBlockMaterialDefinition> UsedMaterials;


	void AddMaterials(int8 n, ...);

};

