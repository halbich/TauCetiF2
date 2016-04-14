

#pragma once

#include "BlockInfo.generated.h"

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
};

struct TAUCETIF2_API FInventoryBlockInfo : FBlockBaseInfo
{

	TArray<FString> Tags;
	FInventoryBlockInfo() : FBlockBaseInfo(), Tags() {};
};


/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UBlockInfo : public UObject
{
	GENERATED_BODY()
public:
	UBlockInfo();
	~UBlockInfo();

	UPROPERTY()
		uint32 ID;

	UPROPERTY()
		FVector Location;

	UPROPERTY()
		FVector Scale;

	UPROPERTY()
		FRotator Rotation;

	FORCEINLINE FBlockInfo ToContainer() {
		FBlockInfo result;
		result.ID = ID;
		result.Location = Location;
		result.Scale = Scale;
		result.Rotation = Rotation;
		return result;
	}

	FORCEINLINE void FromContainer(FBlockInfo& block) {
		ID = block.ID;
		Location = block.Location;
		Scale = block.Scale;
		Rotation = block.Rotation;
	}

};






