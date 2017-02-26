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

	

	FORCEINLINE bool IsSystemAction()
	{
		ensure(BlockDefinition);
		return BlockDefinition->IsSystemAction;
	}
};
