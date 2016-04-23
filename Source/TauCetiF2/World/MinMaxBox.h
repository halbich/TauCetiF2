

#pragma once

#include "Object.h"
#include "Helpers/Helpers.h"
#include "MinMaxBox.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UMinMaxBox : public UObject
{
	GENERATED_BODY()

public:

	UMinMaxBox* InitBox(FVector min, FVector max);
	UMinMaxBox* InitBoxChecked(FVector min, FVector max);

	UPROPERTY()
		UMinMaxBox* ParentNode;

	UPROPERTY()
		FVector Min;

	UPROPERTY()
		FVector Max;

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

	void RemoveElement();

protected:
	template <typename T>
	FORCEINLINE T* GetParentNodeTemplate()
	{
		return ParentNode != nullptr ? Cast<T>(ParentNode) : nullptr;
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
