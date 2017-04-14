#pragma once

#include "Object.h"
#include "MinMaxBox.h"
#include "Commons/Public/GameDefinitions.h"
#include "WeatherTargetsKDTree.generated.h"

class UWeatherTargetsKDTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeatherTargetsChanged, UWeatherTargetsKDTree*, target, bool, isAdding);

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

	UPROPERTY()
		FWeatherTargetsChanged OnWeatherTargetsChanged;

	void DEBUGDrawContainingBox(UWorld* world);
	void DEBUGDrawSurrondings(UWorld* world, FColor usedColor = FColor::Magenta);

	void AddToTree(UWeatherTargetsKDTree* box);

	FORCEINLINE UWeatherTargetsKDTree* GetParent()
	{
		return GetParentNodeTemplate<UWeatherTargetsKDTree>();
	}

	FORCEINLINE UWeatherTargetsKDTree* GetRoot() {
		return GetRootNode<UWeatherTargetsKDTree>(true);
	}

	friend void AddToWeatherTreeElements(UObject* obj, UWeatherTargetsKDTree* box);

	void RemoveFromTree(UObject* obj);

	UPROPERTY(Transient)
		UWeatherTargetsKDTree* rootNode;

private:

	void addToTreeByCoord(UWeatherTargetsKDTree* box);

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
		return A.Max.Z > B.Max.Z;
	}
};