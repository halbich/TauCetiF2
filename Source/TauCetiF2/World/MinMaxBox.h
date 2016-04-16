

#pragma once

#include "Common/WorldObject.h"
#include "Helpers/Helpers.h"

/**
 *
 */
class TAUCETIF2_API FMinMaxBox
{

public:
	FMinMaxBox(FVector min, FVector max, int8 dividingIndex);
	FMinMaxBox(FTransform& transform);
	~FMinMaxBox();

	FVector Min;
	FVector Max;

	FMinMaxBox* B1;

	FMinMaxBox* B2;

	AWorldObject* containingObject;

	int32 DividingIndex;

	FMinMaxBox* SingleChild;

	FString name;

	FVector DividingCoord;

	float DividingCoordValue;

	void DEBUGDrawContainingBox(UWorld* world);

	void AddToTree(FMinMaxBox* box, bool forceIsert = false);
	bool IsPlaceEmpty(const FMinMaxBox& box);


	static FMinMaxBox& FromWorldObject(AWorldObject* object) {

		FVector min;
		FVector max;
		object->GetBoundingBox(min, max);
		auto ret = new FMinMaxBox(min, max, 0);
		ret->containingObject = object;
		return *ret;
	}

	static FMinMaxBox& FromMinMaxBox(FMinMaxBox* boxObject) {

		FVector min;
		FVector max;
		auto ret = new FMinMaxBox(boxObject->Min, boxObject->Max, boxObject->DividingIndex);
		ret->containingObject = boxObject->containingObject;
		ret->name = FString(boxObject->name);
		return *ret;
	}

	FORCEINLINE void RecomputeDividingCoordValue()
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *DividingCoord.ToString());

		FVector dividingCoordVect = (Min + Max) * DividingCoord * 0.5;
		DividingCoordValue = sum(dividingCoordVect);

		if (((int32)FMath::Abs(sum((Max - Min) * DividingCoord)) / UHelpers::CubeMinSize) % 2 == 1)
		{
			DividingCoordValue += UHelpers::CubeMinSize / 2.0f;
		}
	}


private:

	void addToTreeByCoord(FMinMaxBox* box);

	bool isPlaceEmptySingleChild(const FMinMaxBox& box);


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
