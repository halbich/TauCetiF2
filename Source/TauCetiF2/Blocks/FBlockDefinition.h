

#pragma once

#include "Helpers/Helpers.h"

class TAUCETIF2_API FBlockMaterialDefinition
{
public:
	
	EMaterialInstance MaterialInstance;

	EAxis::Type KX;
	EAxis::Type KY;

	bool IsTranslucent;


	FBlockMaterialDefinition(const EMaterialInstance instance, EAxis::Type kX, EAxis::Type kY, bool isTranslucent = false) :
		MaterialInstance(instance),
		KX(kX),
		KY(kY),
		IsTranslucent(isTranslucent)
	{
	};


	FVector2D GetParams(const FVector& scale) {
		FVector2D res;

		switch (KX) {
		case EAxis::Type::X: res.X = scale.X;break;
		case EAxis::Type::Y: res.X = scale.Y;break;
		case EAxis::Type::Z: res.X = scale.Z;break;
		case EAxis::Type::None: res.X = 0; break;
		default: 
			checkNoEntry();
			break;
		}

		switch (KY) {
		case EAxis::Type::X: res.Y = scale.X; break;
		case EAxis::Type::Y: res.Y = scale.Y; break;
		case EAxis::Type::Z: res.Y = scale.Z; break;
		case EAxis::Type::None: res.Y = 0; break;
		default:
			checkNoEntry();
			break;
		}

		return res;

	}

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

