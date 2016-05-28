

#pragma once
#include "Game/GameDefinitions.h"
#include "Blocks/Definitions/FBlockMaterialDefinition.h"
#include "Blocks/Definitions/FBlockAdditionalFlagsDefinition.h"

/**
 *
 */
class TAUCETIF2_API FBlockDefinition
{

public:
	FBlockDefinition(int32 id, EShapeType shapeType);
	FBlockDefinition(EBlockName name, EShapeType shapeType);

	int32 ID;

	EShapeType ShapeType;

	bool AllowPitch;
	bool AllowRoll;
	bool AllowYaw;

private:
	bool HasCustomScaling;
	FVector MeshScale;
	FVector WorldObjectScale;

public:
	FVector MinBlockScale;
	FVector MaxBlockScale;

	TArray<FBlockMaterialDefinition> UsedMaterials;


	void AddMaterials(int8 n, ...);

	int32 BuilderSortIndex;

	TArray<FBlockAdditionalFlagsDefinition> AdditionalFlags;

	FORCEINLINE void SetCustomScale(FVector meshScale, FVector worldObjectScale)
	{
		HasCustomScaling = true;
		MeshScale = meshScale;
		WorldObjectScale = worldObjectScale;
		MinBlockScale = MaxBlockScale = worldObjectScale;
	}

	FORCEINLINE FVector GetMeshScale(const FVector& inScale) const
	{
		return FVector(HasCustomScaling ? MeshScale : inScale);
	}

	FORCEINLINE FVector GetObjectScale(const FVector& inScale) const
	{
		return FVector(HasCustomScaling ? WorldObjectScale : inScale);
	}

	FORCEINLINE bool IsInLimits(FVector dimensions)
	{
		auto scale = HasCustomScaling ? WorldObjectScale : dimensions;

		auto min = MinBlockScale.X <= scale.X && MinBlockScale.Y <= scale.Y && MinBlockScale.Z <= scale.Z;
		auto max = MaxBlockScale.X >= scale.X && MaxBlockScale.Y >= scale.Y && MaxBlockScale.Z >= scale.Z;
		return min && max;
	}

	FORCEINLINE bool ValidateFlags(TArray<FString> flagNames, TArray<int32> flagValues, TArray<FString>& validationErrors)
	{
		for (auto definedFlag : AdditionalFlags)
		{
			auto index = flagNames.IndexOfByKey(definedFlag.TagID);
			if (index == INDEX_NONE)
			{
				validationErrors.Add(TEXT("nenalezen index flagu"));
				return false;
			}

			if (!flagValues.IsValidIndex(index))
			{
				validationErrors.Add(TEXT(" nenalezena hodnota flagu"));
				return false;
			}

			auto value = flagValues[index];

			if (definedFlag.PossibleValues.IndexOfByPredicate([value](const FBlockFlagValue& v) { return v.Value == value; }) == INDEX_NONE)
			{
				validationErrors.Add(TEXT(""));
				return false;
			}

		}

		return true;
	}
};

