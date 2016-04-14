

#pragma once

#include "Helpers/Saving/BlockSaveInfo.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockInfo& block)
{
	Ar << block.ID;
	Ar << block.Location;
	Ar << block.Scale;
	Ar << block.Rotation;

	return Ar;
}