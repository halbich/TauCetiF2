#pragma once

#include "Object.h"
#include "MinMaxBox.generated.h"

#pragma optimize("", off)
/**
 *
 */
UCLASS(BlueprintType)
class BLOCKS_API UMinMaxBox : public UObject
{
	GENERATED_BODY()

public:

	UMinMaxBox* InitBox(FVector min, FVector max);
	UMinMaxBox* InitBoxChecked(FVector min, FVector max);

	UPROPERTY(Transient)
		UMinMaxBox* ParentNode;

	UPROPERTY(Transient)
		FVector Min;

	UPROPERTY(Transient)
		FVector Max;

	UPROPERTY(Transient)
		FVector MinWorldCoord;

	UPROPERTY(Transient)
		FVector MaxWorldCoord;

	void RemoveElement();

	// Return true if both given boxes has common surface
	static bool HasCommonBoundaries(const UMinMaxBox* box1, const UMinMaxBox* box2);

	template <typename T>
	T* GetRootNode(bool allowSelf = false)
	{
		auto returnValue = ParentNode;
		if (returnValue == nullptr)
			return allowSelf ? Cast<T>(this) : nullptr;

		auto newReturn = returnValue->ParentNode;
		while (newReturn)
		{
			returnValue = newReturn;
			newReturn = returnValue->ParentNode;
		}

		return Cast<T>(returnValue);
	}

	FORCEINLINE	void DEBUGDrawBorder(UWorld* world, FColor color = FColor::Magenta)
	{
		if (!world || IsPendingKill())
			return;

		auto bcenter = (Max + Min) * 0.5;
		auto bextend = (Max - bcenter);
		DrawDebugBox(world, bcenter, bextend, color, true);
	}

	FORCEINLINE FBox GetBox()
	{
		FBox ret(Min, Max);
		return ret;
	}

protected:
	template <typename T>
	FORCEINLINE T* GetParentNodeTemplate()
	{
		return ParentNode != nullptr ? Cast<T>(ParentNode) : nullptr;
	}

	FORCEINLINE void SetParent(UMinMaxBox* node)
	{
		ParentNode = node;
	}

	FORCEINLINE bool GtMin(const FVector& min)
	{
		return Min.X <= min.X && Min.Y <= min.Y && Min.Z <= min.Z;
	}

	FORCEINLINE bool LtMax(const FVector& max)
	{
		return Max.X >= max.X && Max.Y >= max.Y && Max.Z >= max.Z;
	}
};

#pragma optimize("", on)