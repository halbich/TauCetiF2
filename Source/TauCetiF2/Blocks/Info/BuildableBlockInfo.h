

#pragma once

#include "Object.h"
#include "Blocks/Info/BlockBaseInfo.h"
#include "BuildableBlockInfo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UBuildableBlockInfo : public UBlockBaseInfo
{
	GENERATED_BODY()
public:


	UPROPERTY()
		TArray<FString> Tags;

	UPROPERTY()
		FRotator DefaultBuildingRotation;

	UPROPERTY(BlueprintReadOnly, Category= BuildableBlockInfo)
		bool IsEmptyHand;

	UPROPERTY(BlueprintReadOnly, Category = BuildableBlockInfo)
		bool AllowOutlineOnSelected;


	FORCEINLINE FInventoryBuildableBlockInfo ToContainer() {
		FInventoryBuildableBlockInfo result = ToBaseContainer();
		result.Tags = Tags;
		result.DefaultBuildingRotation = DefaultBuildingRotation;
		return result;
	}

	FORCEINLINE void FromContainer(FInventoryBuildableBlockInfo& info) {
		FromBaseContainer(info);
		Tags = info.Tags;
		DefaultBuildingRotation = info.DefaultBuildingRotation;
	}



};
