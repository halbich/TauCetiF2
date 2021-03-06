﻿#pragma once

#include "BlockSaveInfo.h"
#include "BlockComponents/BlockComponentsArchiveHelpers.h"
#include "Inventory/InventoryArchiveHelpers.h"
#include "Relations/RelationsArchiveHelpers.h"

FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockBaseInfo& block)
{
	Ar << block.ID;
	Ar << block.Scale;
	Ar << block.Name;
	Ar << block.AdditionalFlags;

	Ar << block.HasOxygenData;
	if (block.HasOxygenData)
		Ar << block.OxygenInfo;

	Ar << block.HasElectricityData;
	if (block.HasElectricityData)
		Ar << block.ElectricityInfo;

	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockInfo& block)
{
	Ar << (FBlockBaseInfo&)block;
	Ar << block.Location;
	Ar << block.Rotation;
	Ar << block.Health;
	Ar << block.BlockSpecificData;

	Ar << block.HasRelationshipData;
	if (block.HasRelationshipData)
		Ar << block.RelationshipInfo;

	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FInventoryBuildableBlockInfo& block)
{
	Ar << (FBlockBaseInfo&)block;
	Ar << block.Tags;
	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FInventoryBuildableItemBlockInfo& block)
{
	Ar << (FInventoryBuildableBlockInfo&)block;
	return Ar;
}
