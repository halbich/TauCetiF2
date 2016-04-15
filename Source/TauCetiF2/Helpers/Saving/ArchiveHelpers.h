

#pragma once

#include "Helpers/Saving/BlockSaveInfo.h"



FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockBaseInfo& block)
{
	Ar << block.ID;
	Ar << block.Scale;
	Ar << block.Name;

	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockInfo& block)
{
	Ar << (FBlockBaseInfo&)block;
	Ar << block.Location;
	Ar << block.Rotation;

	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FInventoryBuildableBlockInfo& block)
{
	Ar << (FBlockBaseInfo&)block;
	Ar << block.Tags;

	return Ar;
}