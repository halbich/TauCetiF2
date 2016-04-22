

#pragma once

#include "Object.h"
#include "MinMaxBox.h"
#include "Helpers/Helpers.h"
#include "KDTree.generated.h"

class AWorldObject;

/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UKDTree : public UMinMaxBox
{
	GENERATED_BODY()

public:

	UKDTree* Init(FVector min, FVector max, int8 dividingIndex);
	UKDTree* Init(UMinMaxBox* box);
	UKDTree* Init(UMinMaxBox* box, UMinMaxBox* constrainingBox);



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
	void DEBUGDrawSurrondings(UWorld* world);

	void AddToTree(UKDTree* box, TArray<UKDTree*>& usedBoxes, bool forceInsert = false);
	bool IsPlaceEmpty(const UMinMaxBox* box);
	void GetContainingObjects(const UMinMaxBox* box, TArray<AWorldObject*>& outArray);
	void GetContainingObjectsFromBottom(const UMinMaxBox* box, TArray<AWorldObject*>& outArray);

private:

	void addToTreeByCoord(UKDTree* box, TArray<UKDTree*>& usedBoxes);

	bool isPlaceEmptySingleChild(const UMinMaxBox* box);

	FORCEINLINE void recomputeDividingCoordValue()
	{

		FVector dividingCoordVect = (Min + Max) * DividingCoord * 0.5;
		DividingCoordValue = sum(dividingCoordVect);

		if (((int32)FMath::Abs(sum((Max - Min) * DividingCoord)) / GameDefinitions::CubeMinSize) % 2 == 1)
		{
			DividingCoordValue += GameDefinitions::CubeMinSize / 2.0f;
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
