

#pragma once


// Help class to save / load data as saveGame
struct TAUCETIF2_API FBlockBaseInfo {

	uint32 ID;
	FVector Scale;
	FString Name;

	FBlockBaseInfo() : ID(0), Scale(FVector::ZeroVector), Name(TEXT("")) {};

};

// Help class to save / load data as saveGame
struct TAUCETIF2_API FBlockInfo : FBlockBaseInfo
{

	FVector Location;
	FRotator Rotation;

	FBlockInfo() : FBlockBaseInfo(), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator) {};

	FBlockInfo(const FBlockBaseInfo& base) : FBlockBaseInfo(base), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator) {};
};

struct TAUCETIF2_API FInventoryBuildableBlockInfo : FBlockBaseInfo
{

	TArray<FString> Tags;
	FInventoryBuildableBlockInfo() : FBlockBaseInfo(), Tags() {};
	FInventoryBuildableBlockInfo(const FBlockBaseInfo& base) : FBlockBaseInfo(base), Tags() {};
};
