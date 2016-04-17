

#pragma once

#include "Helpers/Saving/BlockSaveInfo.h"
#include "Blocks/Info/BlockBaseInfo.h"
#include "BlockInfo.generated.h"




/*
 *	Block info with Location and Scale, used in game. For saving see FBlockInfo
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UBlockInfo : public UBlockBaseInfo
{
	GENERATED_BODY()
public:



	UPROPERTY(BlueprintReadOnly, Category = BlockInfo)
		FVector Location;


	UPROPERTY(BlueprintReadOnly, Category = BlockInfo)
		FRotator Rotation;

	UPROPERTY()
		bool UnderConstruction;

	FORCEINLINE FBlockInfo ToContainer() {
		FBlockInfo result = ToBaseContainer();
		result.Location = Location;
		result.Rotation = Rotation;
		return result;
	}

	FORCEINLINE void FromContainer(const FBlockInfo& block) {

		FromBaseContainer(block);
		Location = block.Location;
		Rotation = block.Rotation;
	}

};






