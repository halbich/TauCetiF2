

#include "TauCetiF2.h"
#include "FBlockDefinition.h"




FBlockDefinition::FBlockDefinition(uint32 id, EShapeType shapeType) :
	ID(id),
	ShapeType(shapeType),
	AllowPitch(true),
	AllowRoll(true),
	AllowYaw(true),
	MinBlockScale(FVector(1, 1, 1)),
	MaxBlockScale(FVector(20, 20, 20))
{

}

FBlockDefinition::FBlockDefinition(EBlockName name, EShapeType shapeType) : FBlockDefinition((uint32)name, shapeType)
{

}


void FBlockDefinition::AddMaterials(int8 n, ...)
{

	UsedMaterials.Reserve(n);
	va_list arguments;

	va_start(arguments, n);
	for (int32 x = 0; x < n; x++)
	{
		auto mat = va_arg(arguments, FBlockMaterialDefinition);
		UsedMaterials.Add(mat);
	}

	va_end(arguments);
}


