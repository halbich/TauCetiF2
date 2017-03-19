#pragma once

#include "Blocks/Public/Tree/MinMaxBox.h"
#include "Blocks/Public/Helpers/BlockHelpers.h"

/**
 *
 */
namespace BlockHelpers1
{
	static UMinMaxBox* GetSpawnBox(const UBlockDefinition* definition, const UBlockInfo* blockInfo)
	{
		auto objectScale = definition->GetObjectScale(blockInfo->Scale).GridSnap(1);
		auto spawnCoord = BlockHelpers::GetSpawnCoords(blockInfo->Location, objectScale, blockInfo->Rotation);
		auto scaleHalf = blockInfo->Rotation.RotateVector(objectScale) * GameDefinitions::CubeMinSize* 0.5;
		return NewObject<UMinMaxBox>()->InitBoxChecked((spawnCoord - scaleHalf).GridSnap(GameDefinitions::CubeMinSizeHalf), (spawnCoord + scaleHalf).GridSnap(GameDefinitions::CubeMinSizeHalf));
	}
};
