

#pragma once

#include "Helpers/Saving/BlockSaveInfo.h"
#include "Blocks/BlockBaseInfo.h"
#include "BlockInfo.generated.h"




/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UBlockInfo : public UBlockBaseInfo
{
	GENERATED_BODY()
public:

	

	UPROPERTY()
		FVector Location;


	UPROPERTY()
		FRotator Rotation;

	FORCEINLINE FBlockInfo ToContainer() {
		FBlockInfo result = ToBaseContainer();
		result.Location = Location;
		result.Rotation = Rotation;
		return result;
	}

	FORCEINLINE void FromContainer(FBlockInfo& block) {

		FromBaseContainer(block);
		Location = block.Location;
		Rotation = block.Rotation;
	}

};






