

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
		result.Add(NewObject<UBuildableBlockInfo>());
		result.Add(NewObject<UBuildableBlockInfo>());

		result[0]->IsEmptyHand = true;
		result[0]->IsSystemAction = true;
		result[0]->Action = EBuildableObjectAction::None;
		result[0]->AllowOutlineOnSelected = false;
		result[0]->Tags.Add(TEXT("hand"));

		result[1]->IsEmptyHand = true;
		result[1]->IsSystemAction = true;
		result[1]->Action = EBuildableObjectAction::DeleteObject;
		result[1]->StencilOverride = STENCIL_DELETE_OUTLINE;
		result[1]->Tags.Add(TEXT("delete"));

		return result;
	}


	static FORCEINLINE UBuildableBlockInfo* GetDefaultBuildableForID(int32 id)
	{
		auto res = NewObject<UBuildableBlockInfo>();
		//TODO
		res->ID = id;

		switch (id)
		{
		case (int32)EBlockName::Door:
			res->DefaultBuildingRotation = FRotator(0, 90, 0);
			break;
		default:
			break;
		}

		return res;
	}


	FORCEINLINE void AddImplicitTags()
	{
		Tags.Add(FString::Printf(TEXT("kX_%d"), (int32)Scale.X));
		Tags.Add(FString::Printf(TEXT("kY_%d"), (int32)Scale.Y));
		Tags.Add(FString::Printf(TEXT("kZ_%d"), (int32)Scale.Z));
	}

	
};
