#pragma once

#include "Blocks/Public/Info/BlockBaseInfo.h"
#include "Blocks/Public/Definitions/BlockDefinition.h"
#include "Commons/Public/GameDefinitions.h"
#include "BlockHolderComponent.h"
#include "BuildableBlockInfo.generated.h"

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
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

	UPROPERTY(BlueprintReadOnly, Transient, Category = BuildableBlockInfo)
		FText DisplayValue;

	UPROPERTY(BlueprintReadOnly, Transient, Category = BuildableBlockInfo)
		FText DisplayValueName;

	UPROPERTY(BlueprintReadOnly, Transient, Category = BuildableBlockInfo)
		float BuildingEnergyRequired;

	bool ValidateObject(TArray<FText>& validationErrors);

	virtual void DefinitionSet();

	static UBuildableBlockInfo* GetBuildable(UBlockDefinition* def);

	UBuildableBlockInfo* GetCopy();

private:

	FORCEINLINE float buildingCoeficient()
	{
		switch (BlockDefinition->ShapeType)
		{
		case EShapeType::Empty: return 1.0f;
		case EShapeType::CubeBody: return 0.5f;
		case EShapeType::CubeSide: return 0.333333f;

		default: return 1.0f;
		}
	}
};
