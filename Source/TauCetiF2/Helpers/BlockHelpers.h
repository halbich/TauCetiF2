

#pragma once
#include "Blocks/Implementations/BaseShapes/CubeObject.h"
#include "Blocks/Implementations/BaseShapes/CubeSideObject.h"
#include "Blocks/Implementations/BaseShapes/CubeBodyObject.h"
#include "Blocks/Implementations/Custom/TerminalObject.h"

/**
 * 
 */
struct TAUCETIF2_API BlockHelpers
{

	static FORCEINLINE UClass* GetClassByShape(const FBlockDefinition& definition) {

		switch (definition.ShapeType)
		{
		case EShapeType::Cube: return ACubeObject::StaticClass();
		case EShapeType::CubeSide: return ACubeSideObject::StaticClass();
		case EShapeType::CubeBody: return ACubeBodyObject::StaticClass();
		case EShapeType::Custom: {

			if (definition.ID == (uint32)EBlockName::Terminal)
				return ATerminalObject::StaticClass();

			break;
		}

		default:

			return nullptr;
			break;
		}

		return nullptr;
	}

	

};
