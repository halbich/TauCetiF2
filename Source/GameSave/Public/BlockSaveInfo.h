﻿#pragma once

#include "BlockComponents/OxygenComponentInfo.h"
#include "BlockComponents/ElectricityComponentInfo.h"
#include "Relations/FBlockWithRelationshipInfo.h"
#include "Commons/Public/GameDefinitions.h"

// Help class to save / load data as saveGame

/*
 Base info for blocks, used to load / save to save game
*/
struct FBlockBaseInfo {
	int32 ID;
	FVector Scale;
	FString Name;
	TMap<FString, int32> AdditionalFlags;

	bool HasOxygenData;
	FOxygenComponentInfo OxygenInfo;
	bool HasElectricityData;
	FElectricityComponentInfo ElectricityInfo;

	/*
		Default constructor with 0 ID, Zero scale, Empty name and component data
	*/
	FBlockBaseInfo() : ID(0), Scale(FVector::ZeroVector), Name(TEXT("")), AdditionalFlags(),
		HasOxygenData(false), HasElectricityData(false), OxygenInfo(), ElectricityInfo()
	{};
};

/*
	Info for blocks with block location and rotation anc componentData, used to load / save to save game
*/
struct FBlockInfo : FBlockBaseInfo
{
	FVector Location;
	FRotator Rotation;
	float Health;
	TMap<FString, FString> BlockSpecificData;

	bool HasRelationshipData;
	FBlockWithRelationshipInfo RelationshipInfo;

	FBlockInfo() : FBlockBaseInfo(),
		Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), Health(0), BlockSpecificData(),
		HasRelationshipData(false), RelationshipInfo()
	{};

	FBlockInfo(const FBlockBaseInfo& base) : FBlockBaseInfo(base),
		Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), Health(0), BlockSpecificData(),
		HasRelationshipData(false), RelationshipInfo()
	{};
};

/*
	Info about blocks which can be built by player
*/
struct FInventoryBuildableBlockInfo : FBlockBaseInfo
{
	TArray<FString> Tags;
	FInventoryBuildableBlockInfo() : FBlockBaseInfo(), Tags() {};
	FInventoryBuildableBlockInfo(const FBlockBaseInfo& base) : FBlockBaseInfo(base), Tags() {};

	FORCEINLINE void AddImplicitTags()
	{
		if (ID == DeleteID)
		{
			Tags.Add(NSLOCTEXT("TCF2LocSpace", "LC.DeleteAction.Tag", "smazat").ToString());
			return;
		}

		Tags.Add(FString::Printf(TEXT("kX_%d"), (int32)Scale.X));
		Tags.Add(FString::Printf(TEXT("kY_%d"), (int32)Scale.Y));
		Tags.Add(FString::Printf(TEXT("kZ_%d"), (int32)Scale.Z));
	}
};

/*
Info about blocks which nas player in inventory
*/
struct FInventoryBuildableItemBlockInfo : FInventoryBuildableBlockInfo
{
};