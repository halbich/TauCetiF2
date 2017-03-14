#pragma once

#include "Object.h"
#include "Commons/Public/Enums.h"
#include "BlockMeshStructureDefinition.h"
#include "BlockAdditionalFlags.h"
#include "OxygenComponentDefinition.h"
#include "ElectricityComponentDefinition.h"
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
		IsBuildable = true;
		IsPlaceable = true;
		IsInventoryObject = false;
	};

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BuildDefinition")
		FName GamePackageName;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BuildDefinition")
		int32 BlockID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | BuildDefinition")
		UTexture2D* BlockImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | BuildDefinition")
		bool IsSystemAction;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BuildDefinition")
		bool IsBuildable;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BuildDefinition")
		bool IsPlaceable;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BuildDefinition")
		bool IsInventoryObject;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BuildDefinition")
		bool AllowUsable;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | BlockDefinition", meta = (EditCondition = "!HasCustomScaling"))
		FVector MinBlockScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TCF2 | BlockDefinition", meta = (EditCondition = "!HasCustomScaling"))
		FVector MaxBlockScale;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition", meta = (ShowOnlyInnerProperties))
		TArray<FBlockAdditionalFlags> AdditionalFlags;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition")
		FRotator DefaultBuildingRotation;

	// The result is equal: This value * Coeficient by type * HasCustomScaling ? (1*1*1) : Scale 
	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | BlockDefinition")
		float EnergyReqiredToBuild;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | OxygenComponent")
		bool HasOxygenComponent;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | OxygenComponent", meta = (ShowOnlyInnerProperties, EditCondition = "HasOxygenComponent"))
		FOxygenComponentDefinition OxygenComponentDef;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | ElectricityComponent")
		bool HasElectricityComponent;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | ElectricityComponent", meta = (ShowOnlyInnerProperties, EditCondition = "HasElectricityComponent"))
		FElectricityComponentDefinition ElectricityComponentDef;

	FVector GetMeshScale(const FVector& inScale) const
	{
		return HasCustomScaling ? FVector(1, 1, 1) : inScale;
	}

	FVector GetObjectScale(const FVector& inScale) const
	{
		return FVector(HasCustomScaling ? CustomBlockScale : inScale);
	}

	FORCEINLINE bool IsInLimits(FVector dimensions)
	{
		auto scale = HasCustomScaling ? CustomBlockScale : dimensions;

		auto minS = MinBlockScale;
		auto maxS = MaxBlockScale;

		auto min = minS.X <= scale.X && minS.Y <= scale.Y && minS.Z <= scale.Z;
		auto max = maxS.X >= scale.X && maxS.Y >= scale.Y && maxS.Z >= scale.Z;
		return min && max;
	}

	FORCEINLINE bool ValidateFlags(TMap<FString, int32> flags, TArray<FText>& validationErrors)
	{
		for (auto definedFlag : AdditionalFlags)
		{
			auto value = flags.Find(definedFlag.TagID);
			if (!value)
			{
				validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.BlockDefinition.InvalidFlag", "Chyba v bloku s ID {0}: Požadovaný flag '{1}' nebyl nalezen."), BlockID, FText::FromString(definedFlag.TagID)));
				return false;
			}

			if (definedFlag.PossibleValues.IndexOfByPredicate([value](const FBlockFlagValue& v) { return v.Value == *value; }) == INDEX_NONE)
			{
				validationErrors.Add(FText::Format(NSLOCTEXT("TCF2LocSpace", "LC.BlockDefinition.InvalidFlagValue", "Chyba v bloku s ID {0}: Hodnota flagu '{1}' není platná."), BlockID, *value));
				return false;
			}
		}

		return true;
	}
};
