#pragma once

#include "Object.h"
#include "MinMaxBox.h"
#include "Commons/Public/GameDefinitions.h"
#include "WeatherTargetsKDTree.generated.h"





/**
 *
 */
UCLASS(BlueprintType)
class BLOCKS_API UWeatherTargetsKDTree : public UMinMaxBox
{
	GENERATED_BODY()

public:

	UWeatherTargetsKDTree* Init(FVector min, FVector max, int8 dividingIndex);
	UWeatherTargetsKDTree* Init(UMinMaxBox* box);
	UWeatherTargetsKDTree* Init(UMinMaxBox* box, UMinMaxBox* constrainingBox);

	UPROPERTY(Transient)
		UWeatherTargetsKDTree* B1;

	UPROPERTY(Transient)
		UWeatherTargetsKDTree* B2;

	UPROPERTY(Transient)
		UObject* ContainingObject;

	UPROPERTY(Transient)
		int32 DividingIndex;

	UPROPERTY(Transient)
		TArray<UWeatherTargetsKDTree*> ChildHeap;

	UPROPERTY(Transient)
		FVector DividingCoord;

	UPROPERTY(Transient)
		float DividingCoordValue;

	void DEBUGDrawContainingBox(UWorld* world);
	void DEBUGDrawSurrondings(UWorld* world, FColor usedColor = FColor::Magenta);

	void AddToTree(UWeatherTargetsKDTree* box);
	bool IsPlaceEmpty(const UMinMaxBox* box);
	void GetContainingObjects(const UMinMaxBox* box, TArray<UObject*>& outArray, const UObject* ignoreElement = nullptr);
	void GetContainingObjectsFromBottom(const UMinMaxBox* box, TArray<UObject*>& outArray, const UObject* ignoreElement);

	void UpdateAfterChildDestroyed();

	FORCEINLINE UWeatherTargetsKDTree* GetParent()
	{
		return GetParentNodeTemplate<UWeatherTargetsKDTree>();
	}

	//friend bool CheckCommonBoundaries1(UObject* o1, const UObject* o2);
	//friend void AddToTreeElements1(UObject* obj, UWeatherTargetsKDTree* box);
	//friend void RemoveFromTreeElements1(UObject* obj, UWeatherTargetsKDTree* box);


private:

	void addToTreeByCoord(UWeatherTargetsKDTree* box);

	bool isPlaceEmptySingleChild(const UMinMaxBox* box);

	void updateAfterChildDestroyedInner();

	FORCEINLINE void recomputeDividingCoordValue()
	{
		FVector dividingCoordVect = (Min + Max) * DividingCoord * 0.5;
		DividingCoordValue = sum(dividingCoordVect);

		if (((int32)FMath::Abs(sum((Max - Min) * DividingCoord)) / GameDefinitions::CubeMinSize) % 2 == 1)
		{
			DividingCoordValue += GameDefinitions::CubeMinSize / 2.0f;
		}
	}

	FORCEINLINE float sum(const FVector& v)
	{
		return v.X + v.Y + v.Z;
	}

	FORCEINLINE bool canBeDeleted() {
		return GetParent() != nullptr;
	}

	FORCEINLINE bool checkElem(UWeatherTargetsKDTree*& elem)
	{
		auto hasElem = false;
		if (elem && elem->IsValidLowLevel())
		{
			if (!elem->IsPendingKill())
				hasElem = true;
			else
				elem = nullptr;
		}
		return hasElem;
	}
};

// struct for heap sorting
struct WeatherTargetsPriorityPredicate
{
	bool operator() (const UWeatherTargetsKDTree& A, const UWeatherTargetsKDTree& B) const
	{
		return A.Max.Z < B.Max.Z;
	}
};