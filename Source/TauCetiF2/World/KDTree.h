

#pragma once

#include "Object.h"
#include "MinMaxBox.h"
#include "Blocks/WorldObject.h"
#include "Helpers/Helpers.h"
#include "KDTree.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UKDTree : public UMinMaxBox
{
	GENERATED_BODY()

public:

	UKDTree* Init(FVector min, FVector max, int8 dividingIndex);
	UKDTree* Init(FTransform& transform);


	UPROPERTY()
		UKDTree* B1;

	UPROPERTY()
		UKDTree* B2;

	UPROPERTY()
		AWorldObject* containingObject;

	UPROPERTY()
		int32 DividingIndex;

	UPROPERTY()
		UKDTree* SingleChild;

	UPROPERTY()
		FVector DividingCoord;

	UPROPERTY()
		float DividingCoordValue;

	void DEBUGDrawContainingBox(UWorld* world);

	void AddToTree(UKDTree* box, bool forceInsert = false);
	bool IsPlaceEmpty(const UMinMaxBox* box);

private:

	void addToTreeByCoord(UKDTree* box);

	bool isPlaceEmptySingleChild(const UMinMaxBox* box);

	FORCEINLINE void recomputeDividingCoordValue()
	{

		FVector dividingCoordVect = (Min + Max) * DividingCoord * 0.5;
		DividingCoordValue = sum(dividingCoordVect);

		if (((int32)FMath::Abs(sum((Max - Min) * DividingCoord)) / UHelpers::CubeMinSize) % 2 == 1)
		{
			DividingCoordValue += UHelpers::CubeMinSize / 2.0f;
		}
	}

	FORCEINLINE bool GtMin(const FVector& min)
	{
		return Min.X <= min.X && Min.Y <= min.Y && Min.Z <= min.Z;
	}

	FORCEINLINE bool LtMax(const FVector& max)
	{
		return Max.X >= max.X && Max.Y >= max.Y && Max.Z >= max.Z;
	}

	FORCEINLINE float sum(const FVector& v)
	{
		return v.X + v.Y + v.Z;
	}

};
