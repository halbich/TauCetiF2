

#pragma once

#include "Object.h"
#include "Commons/Public/Enums.h"
#include "BlockMeshStructureDefinition.h"
#include "BlockAdditionalFlags.h"
#include "BlockDefinition.generated.h"


namespace BuildDefinition {

	const FName DefaultGamePackage("TCF2Default");
}



/**
 * Class for definition of how block behave in game
 */
UCLASS(Blueprintable)
class BLOCKS_API UBlockDefinition : public UObject
{
	GENERATED_BODY()

public:

	UBlockDefinition() {
		GamePackageName = BuildDefinition::DefaultGamePackage;

		CustomBlockScale = FVector(1, 1, 1);
		MinBlockScale = FVector(1, 1, 1);
		MaxBlockScale = FVector(20, 20, 20);
		AllowPitch = AllowRoll = AllowYaw = true;
	};

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BuildDefinition")
		FName GamePackageName;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BuildDefinition")
		int32 BlockID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | BuildDefinition")
		UTexture2D* BlockImage;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | BuildDefinition")
		bool IsEmptyHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | BuildDefinition")
		bool IsSystemAction;


	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition")
		EShapeType ShapeType;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition")
		TArray<FBlockMeshStructureDefinition> MeshStructure;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition")
		bool AllowPitch;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition")
		bool AllowRoll;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition")
		bool AllowYaw;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition")
		bool HasCustomScaling;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition", meta = (EditCondition = HasCustomScaling))
		FVector CustomBlockScale;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition", meta = (EditCondition = "!HasCustomScaling"))
		FVector MinBlockScale;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition", meta = (EditCondition = "!HasCustomScaling"))
		FVector MaxBlockScale;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition", meta = (ShowOnlyInnerProperties))
		TArray<FBlockAdditionalFlags> AdditionalFlags;



	FVector GetMeshScale(const FVector& inScale) const
	{
		// TODO opravit custom scaling
		return FVector(HasCustomScaling ? inScale : inScale);
	}

	FVector GetObjectScale(const FVector& inScale) const
	{
		// TODO opravit custom scaling
		return FVector(HasCustomScaling ? CustomBlockScale : inScale);
	}

	FORCEINLINE bool IsInLimits(FVector dimensions)
	{
		// TODO opravit custom scaling
		auto scale = HasCustomScaling ? CustomBlockScale : dimensions;

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
