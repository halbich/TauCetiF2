﻿#pragma once

#include "Object.h"
#include "Components/BlockWithOxygenInfo.h"
#include "Components/BlockWithElectricityInfo.h"
#include "BlockBaseInfo.generated.h"

/*
 *	Base info about block, used in game. (Not used for saving, see FBlockBaseInfo)
 */
UCLASS(BlueprintType)
class BLOCKS_API UBlockBaseInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | BlockInfo")
		int32 ID;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | BlockInfo")
		FVector Scale;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | BlockInfo")
		FString Name;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | BlockInfo")
		TMap<FString, int32> AdditionalFlags;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		UBlockWithOxygenInfo* OxygenInfo;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BlockInfo")
		UBlockWithElectricityInfo* ElectricityInfo;
};
