

#pragma once

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


	

	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockInfo)
		bool UnderConstruction;
};






