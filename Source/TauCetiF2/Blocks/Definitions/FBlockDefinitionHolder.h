

#pragma once

#include "Game/GameDefinitions.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockMaterialDefinition.h"

/**
 *
 */
class TAUCETIF2_API FBlockDefinitionHolder
{
public:


	static FBlockDefinitionHolder& Instance();

	FORCEINLINE FBlockDefinition* GetDefinition(int32 id, bool ensure = true) {
		auto definition = definitions.Find(id);

		if (!ensure)
			return definition;

		ensureMsgf(definition != nullptr, TEXT("Definition (%d) was not found!"), id);

		return definition;
	}



	FORCEINLINE bool ShapeCanBeSubdivided(const EShapeType type)
	{
		return type == EShapeType::CubeSide;
	}

	FORCEINLINE FVector GetSubdivisionCoords(const EShapeType type)
	{
		ensure(ShapeCanBeSubdivided(type));
		switch (type)
		{
		case EShapeType::CubeSide:
			return FVector(0, 1, 1);
		default:
			checkNoEntry();
			return FVector::ZeroVector;
			break;
		}
	}

	// TODO splitter bloku

	void GetAllDefinitions(TArray<int32>& outArray);

private:

	FBlockDefinitionHolder();

	FBlockDefinitionHolder(const FBlockDefinitionHolder&) = delete;
	FBlockDefinitionHolder& operator= (const FBlockDefinitionHolder&) = delete;

	TMap<int32, FBlockDefinition> definitions;
};
