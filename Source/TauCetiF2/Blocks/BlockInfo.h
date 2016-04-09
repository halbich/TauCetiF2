

#pragma once

//#include "FBlockInfo.generated.h"

/**
 *
 */
 //UCLASS()
class TAUCETIF2_API FBlockInfo// : public UObject
{
	//GENERATED_BODY()
public:
	FBlockInfo();
	~FBlockInfo();

	uint32 ID;
	FVector Location;
	FVector BlockScale;
	FRotator BlockRotation;

	EShapeType ShapeType;

};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FBlockInfo* block)
{
	if (!block)
		return Ar;
	//~

	Ar << block->ID;
	Ar << block->Location;
	Ar << block->BlockScale;
	Ar << block->BlockRotation;
	Ar << (uint8&)block->ShapeType;

	return Ar;
}
