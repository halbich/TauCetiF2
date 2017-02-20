

#pragma once

//#include "Helpers/Saving/BlockSaveInfo.h"
#include "BlockBaseInfo.h"
#include "BlockInfo.generated.h"




/*
 *	Block info with Location and Scale, used in game. For saving see FBlockInfo
 */
UCLASS(BlueprintType)
class BLOCKS_API UBlockInfo : public UBlockBaseInfo
{
	GENERATED_BODY()
public:



	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockInfo)
		FVector Location;


	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockInfo)
		FRotator Rotation;


	/*FORCEINLINE FBlockInfo ToContainer() {
		FBlockInfo result = ToBaseContainer();
		result.Location = Location;
		result.Rotation = Rotation;
		return result;
	}

	FORCEINLINE void FromContainer(const FBlockInfo& block) {

		FromBaseContainer(block);
		Location = block.Location;
		Rotation = block.Rotation;
	}*/

	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockInfo)
		bool UnderConstruction;
};






