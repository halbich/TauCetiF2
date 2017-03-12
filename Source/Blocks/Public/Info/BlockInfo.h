#pragma once

#include "BlockBaseInfo.h"
#include "Components/BlockWithOxygenInfo.h"
#include "Components/BlockWithElectricityInfo.h"
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

	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockInfo)
		UBlockWithOxygenInfo* OxygenInfo;

	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockInfo)
		UBlockWithElectricityInfo* ElectricityInfo;
};
