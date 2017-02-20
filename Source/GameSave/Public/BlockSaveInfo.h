

#pragma once


// Help class to save / load data as saveGame

/*
 Base info for blocks, used to load / save to save game
*/
struct GAMESAVE_API FBlockBaseInfo {

	int32 ID;
	FVector Scale;
	FString Name;
	TMap<FString,int32> AdditionalFlags;

	/*
		Default constructor with 0 ID, Zero scale and empty name
	*/
	FBlockBaseInfo() : ID(0), Scale(FVector::ZeroVector), Name(TEXT("")), AdditionalFlags() {};

};

/*
	Info for blocks with block location and rotation, used to load / save to save game
*/
struct GAMESAVE_API FBlockInfo : FBlockBaseInfo
{

	FVector Location;
	FRotator Rotation;

	FBlockInfo() : FBlockBaseInfo(), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator) {};
	FBlockInfo(const FBlockBaseInfo& base) : FBlockBaseInfo(base), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator) {};
};

/*
	Info about blocks which can be built by player
*/
struct GAMESAVE_API FInventoryBuildableBlockInfo : FBlockBaseInfo
{

	TArray<FString> Tags;
	FInventoryBuildableBlockInfo() : FBlockBaseInfo(), Tags() {};
	FInventoryBuildableBlockInfo(const FBlockBaseInfo& base) : FBlockBaseInfo(base), Tags() {};
};
