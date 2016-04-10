

#pragma once

#include "BlockInfo.generated.h"


// Help class to save / load data as saveGame
struct TAUCETIF2_API FBlockInfo
{

	uint32 ID;
	FVector Location;
	FVector BlockScale;
	FRotator BlockRotation;

	EShapeType ShapeType;

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

	uint32 ID;
	FVector Location;
	FVector BlockScale;
	FRotator BlockRotation;

	EShapeType ShapeType;

	FORCEINLINE FBlockInfo ToContainer() {
		FBlockInfo result;
		result.ID = ID;
		result.Location = Location;
		result.BlockScale = BlockScale;
		result.BlockRotation = BlockRotation;
		result.ShapeType = ShapeType;
		return result;
	}

	FORCEINLINE void FromContainer(FBlockInfo& block) {
		ID = block.ID;
		Location = block.Location;
		BlockScale = block.BlockScale;
		BlockRotation = block.BlockRotation;
		ShapeType = block.ShapeType;
	}

};






