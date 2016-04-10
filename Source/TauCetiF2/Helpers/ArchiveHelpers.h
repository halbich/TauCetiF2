

#pragma once

#include "Blocks/BlockInfo.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockInfo& block)
{
	Ar << block.ID;
	Ar << block.Location;
	Ar << block.BlockScale;
	Ar << block.BlockRotation;
	Ar << (uint8&)block.ShapeType;
	Ar << (uint8&)block.BlockType;
	Ar << (uint8&)block.MaterialType;

	return Ar;
}