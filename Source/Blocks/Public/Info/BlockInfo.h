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

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		FVector Location;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		FRotator Rotation;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		bool UnderConstruction;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		float Health;
};
