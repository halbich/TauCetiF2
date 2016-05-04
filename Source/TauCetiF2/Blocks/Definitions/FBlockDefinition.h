

#pragma once
#include "Game/GameDefinitions.h"
#include "Blocks/Definitions/FBlockMaterialDefinition.h"

/**
 *
 */
class TAUCETIF2_API FBlockDefinition
{

public:
	FBlockDefinition(int32 id, EShapeType shapeType);
	FBlockDefinition(EBlockName name, EShapeType shapeType);

	int32 ID;

	EShapeType ShapeType;

	bool AllowPitch;
	bool AllowRoll;
	bool AllowYaw;

private:
	bool HasCustomScaling;
	FVector MeshScale;
	FVector WorldObjectScale;

public:
	FVector MinBlockScale;
	FVector MaxBlockScale;

	TArray<FBlockMaterialDefinition> UsedMaterials;


	void AddMaterials(int8 n, ...);

	int32 BuilderSortIndex;

	FORCEINLINE void SetCustomScale(FVector meshScale, FVector worldObjectScale)
	{
		HasCustomScaling = true;
		MeshScale = meshScale;
		WorldObjectScale = worldObjectScale;
	}

	FORCEINLINE FVector GetMeshScale(const FVector& inScale) const
	{
		return FVector(HasCustomScaling ? MeshScale : inScale);
	}

	FVector GetObjectScale(const FVector& inScale) const
	{
		return FVector(HasCustomScaling ? WorldObjectScale : inScale);
	}
};

