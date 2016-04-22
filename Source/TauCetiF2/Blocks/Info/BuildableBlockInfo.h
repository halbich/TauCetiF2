

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
	UBuildableBlockInfo();

	UPROPERTY()
		TArray<FString> Tags;

	UPROPERTY()
		FRotator DefaultBuildingRotation;

	UPROPERTY(BlueprintReadOnly, Category= BuildableBlockInfo)
		bool IsEmptyHand;

	UPROPERTY(BlueprintReadOnly, Category = BuildableBlockInfo)
		EBuildableObjectAction Action;

	UPROPERTY(BlueprintReadOnly, Category = BuildableBlockInfo)
		bool IsSystemAction;

	UPROPERTY(BlueprintReadOnly, Category = BuildableBlockInfo)
		bool AllowOutlineOnSelected;

	UPROPERTY(BlueprintReadOnly, Category = BuildableBlockInfo)
		int32 StencilOverride;


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
