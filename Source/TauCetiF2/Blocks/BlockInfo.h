

#pragma once

#include "Helpers/Saving/BlockSaveInfo.h"
#include "BlockInfo.generated.h"




/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UBlockInfo : public UObject
{
	GENERATED_BODY()
public:

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






