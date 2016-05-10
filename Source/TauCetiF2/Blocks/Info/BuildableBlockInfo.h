

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

	UPROPERTY(BlueprintReadOnly, Category = BuildableBlockInfo)
		TArray<FString> Tags;

	UPROPERTY()
		FRotator DefaultBuildingRotation;

	UPROPERTY(BlueprintReadOnly, Category = BuildableBlockInfo)
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

	static FORCEINLINE TArray<UBuildableBlockInfo*> GetSystemActions()
	{
		TArray<UBuildableBlockInfo*> result;
		result.AddDefaulted(2);
		
		result[0]->IsEmptyHand = true;
		result[0]->IsSystemAction = true;
		result[0]->Action = EBuildableObjectAction::None;
		result[0]->AllowOutlineOnSelected = false;

		result[1]->IsEmptyHand = true;
		result[1]->IsSystemAction = true;
		result[1]->Action = EBuildableObjectAction::DeleteObject;
		result[1]->StencilOverride = STENCIL_DELETE_OUTLINE;

		return result;
	}


	static FORCEINLINE UBuildableBlockInfo* GetDefaultBuildableForID(int32 id)
	{
		auto res = NewObject<UBuildableBlockInfo>();
		//TODO
		return res;
	}

};
