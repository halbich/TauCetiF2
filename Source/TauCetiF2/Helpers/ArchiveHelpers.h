

#pragma once

#include "Blocks/BlockInfo.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockInfo& block)
{
	Ar << block.ID;
	Ar << block.Location;
	Ar << block.Scale;
	Ar << block.Rotation;

	return Ar;
}