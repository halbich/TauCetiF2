#pragma once

#include "Object.h"
#include "BlockBaseInfo.generated.h"

/*
 *	Base info about block, used in game. (Not used for saving, see FBlockBaseInfo)
 */
UCLASS(BlueprintType)
class BLOCKS_API UBlockBaseInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
		int32 ID;

	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockInfo)
		FVector Scale;

	UPROPERTY(BlueprintReadWrite, Transient, Category = BlockInfo)
		FString Name;

	UPROPERTY(Transient)
		TMap<FString, int32> AdditionalFlags;
};
