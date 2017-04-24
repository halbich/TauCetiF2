#pragma once

#include "Blocks/Public/Info/BlockBaseInfo.h"
#include "Blocks/Public/Definitions/BlockDefinition.h"
#include "Commons/Public/GameDefinitions.h"
#include "Commons/Public/TCF2GameInstance.h"
#include "BlockHolder.h"
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

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | BuildableBlockInfo")
		TArray<FString> Tags;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BuildableBlockInfo")
		EBuildableObjectAction Action;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BuildableBlockInfo")
		bool AllowOutlineOnSelected;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BuildableBlockInfo")
		int32 StencilOverride;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BuildableBlockInfo")
		UBlockDefinition* BlockDefinition;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BuildableBlockInfo")
		FText DisplayValue;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BuildableBlockInfo")
		FText DisplayValueName;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BuildableBlockInfo")
		float BuildingEnergyRequired;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | BuildableBlockInfo")
		bool BlockConstructorDisabled;

	bool ValidateObject(TArray<FText>& validationErrors, UBlockHolder* holder);

	virtual void DefinitionSet();

	static UBuildableBlockInfo* GetBuildable(UBlockDefinition* def);

	UBuildableBlockInfo* GetCopy();

	FORCEINLINE float buildingCoeficient()
	{
		switch (BlockDefinition->ShapeType)
		{
		case EShapeType::Empty: return 1.0f;
		case EShapeType::CubeBody: return 0.5f;
		case EShapeType::CubeSide: return (1.0f / 3.0f);

		default: return 1.0f;
		}
	}
};
