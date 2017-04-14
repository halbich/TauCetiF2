#pragma once

#include "Block.h"
#include "Components/ElectricityComponent.h"
#include "BlockWithElectricity.h"
#include "ControllableBlock.h"
#include "Creator.generated.h"

#pragma optimize("", off)

/**
 *
 */
UCLASS()
class BLOCKS_API ACreator : public ABlock, public IBlockWithElectricity, public IControllableBlock
{
	GENERATED_BODY()

public:
	ACreator();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CreatorBlock")
		UStaticMeshComponent* CreatorMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | CreatorBlock", meta = (AllowPrivateAcces = "true"))
		UElectricityComponent* ElectricityComponent;

	UPROPERTY(Transient)
		UMinMaxBox* watchingBox;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | CreatorBlock")
		bool IsValidCreator;

	virtual UPrimitiveComponent* GetComponentForObjectOutline_Implementation() override;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual UMinMaxBox* GetWatchingBox() override;

	virtual void CheckWatchingBox() override;

	virtual void SetControlState_Implementation(bool isOn) override;
	virtual void SetOutputPowerPercentage_Implementation(float percentage) override;
	virtual void SetController_Implementation(ABlock* controller) override;
	virtual ABlock* GetController_Implementation() override;

	FORCEINLINE virtual UElectricityComponent* GetElectricityComponent() override
	{
		return ElectricityComponent;
	}

private:
	bool IntersectExcl(const FBox& box, const FBox& other) const
	{
		auto tmin = FVector(FMath::Max(box.Min.X, other.Min.X), FMath::Max(box.Min.Y, other.Min.Y), FMath::Max(box.Min.Z, other.Min.Z));
		auto tmax = FVector(FMath::Min(box.Max.X, other.Max.X), FMath::Min(box.Max.Y, other.Max.Y), FMath::Min(box.Max.Z, other.Max.Z));

		FBox bx(tmin, tmax);

		if (bx.IsValid == 0)
			return false;

		auto volume = bx.GetVolume();

		return volume == FMath::Pow(GameDefinitions::CubeMinSize, 3);
	}

	TArray<UMinMaxBox*> getAllContaining(const UMinMaxBox* source)
	{
		TArray<UMinMaxBox*> result;
		for (float x = source->Min.X; x < source->Max.X; x += GameDefinitions::CubeMinSize)
			for (float y = source->Min.Y; y < source->Max.Y; y += GameDefinitions::CubeMinSize)
				for (float z = source->Min.Z; z < source->Max.Z; z += GameDefinitions::CubeMinSize)
				{
					auto min = FVector(x, y, z);
					auto max = min + GameDefinitions::CubeMinSize;

					auto toAdd = NewObject<UMinMaxBox>();
					toAdd->InitBox(min, max);
					result.Add(toAdd);
				}

		return result;
	}

	bool checkForBlock(const UMinMaxBox* source, const TMap<ABlock*, FBox>& objectBoxes)
	{
		auto checkContaining = getAllContaining(source);

		// wee need to ensure validity for all blocks in defined region
		for (auto check : checkContaining) {
			auto box = check->GetBox();

			bool hasValidBlock = false;
			for (auto obj : objectBoxes)
			{
				if (obj.Value.Intersect(box) && IntersectExcl(obj.Value, box))
				{
					hasValidBlock = true;
					break;
				}
			}

			if (!hasValidBlock)
				return false;
		}
		return true;
	}
};

#pragma optimize("", on)