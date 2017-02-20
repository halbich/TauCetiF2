

#pragma once


#include "Blocks/Public/Info/BlockBaseInfo.h"
#include "Blocks/Public/Definitions/BlockDefinition.h"
#include "Commons/Public/GameDefinitions.h"
#include "BuildableBlockInfo.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class BLOCKS_API UBuildableBlockInfo : public UBlockBaseInfo
{
	GENERATED_BODY()
public:
	UBuildableBlockInfo();

	UPROPERTY(BlueprintReadWrite, Transient, Category = BuildableBlockInfo)
		TArray<FString> Tags;


	UPROPERTY(BlueprintReadOnly, Transient, Category = BuildableBlockInfo)
		EBuildableObjectAction Action;


	UPROPERTY(BlueprintReadOnly, Transient, Category = BuildableBlockInfo)
		bool AllowOutlineOnSelected;

	UPROPERTY(BlueprintReadOnly, Transient, Category = BuildableBlockInfo)
		int32 StencilOverride;

	UPROPERTY(BlueprintReadOnly, Transient, Category = BuildableBlockInfo)
		UBlockDefinition* BlockDefinition;


	//FORCEINLINE FInventoryBuildableBlockInfo ToContainer() {
	//	FInventoryBuildableBlockInfo result; // = ToBaseContainer();
	//	//TODO
	//	result.Tags = Tags;
	//	return result;
	//}

	//FORCEINLINE void FromContainer(FInventoryBuildableBlockInfo& info) {
	//	//FromBaseContainer(info);
	//	Tags = info.Tags;
	//}

	static FORCEINLINE TArray<UBuildableBlockInfo*> GetSystemActions()
	{
		TArray<UBuildableBlockInfo*> result;
		result.Add(NewObject<UBuildableBlockInfo>());
		result.Add(NewObject<UBuildableBlockInfo>());

		result[0]->Action = EBuildableObjectAction::None;
		result[0]->AllowOutlineOnSelected = false;
		result[0]->Tags.Add(TEXT("hand"));
		result[0]->ID = -10;

		result[1]->Action = EBuildableObjectAction::DeleteObject;
		result[1]->StencilOverride = STENCIL_DELETE_OUTLINE;
		result[1]->Tags.Add(TEXT("delete"));
		result[1]->ID = -5;

		return result;
	}


	FORCEINLINE void AddImplicitTags()
	{
		Tags.Add(FString::Printf(TEXT("kX_%d"), (int32)Scale.X));
		Tags.Add(FString::Printf(TEXT("kY_%d"), (int32)Scale.Y));
		Tags.Add(FString::Printf(TEXT("kZ_%d"), (int32)Scale.Z));
	}

	FORCEINLINE bool IsSystemAction()
	{
		ensure(BlockDefinition);
		return BlockDefinition->IsSystemAction;
	}
};
