#pragma once

#include "Object.h"
#include "MinMaxBox.h"
#include "Commons/Public/GameDefinitions.h"
#include "KDTree.generated.h"



/**
 *
 */
UCLASS(BlueprintType)
class BLOCKS_API UKDTree : public UMinMaxBox
{
	GENERATED_BODY()

public:

	UKDTree* Init(FVector min, FVector max, int8 dividingIndex);
	UKDTree* Init(UMinMaxBox* box);
	UKDTree* Init(UMinMaxBox* box, UMinMaxBox* constrainingBox);

	UPROPERTY(Transient)
		UKDTree* B1;

	UPROPERTY(Transient)
		UKDTree* B2;

	UPROPERTY(Transient)
		UObject* ContainingObject;

	UPROPERTY(Transient)
		int32 DividingIndex;

	UPROPERTY(Transient)
		UKDTree* SingleChild;

	UPROPERTY(Transient)
		FVector DividingCoord;

	UPROPERTY(Transient)
		float DividingCoordValue;

	UPROPERTY(Transient)
		TMap<UObject*, UMinMaxBox*> watchedBoxes;

	void DEBUGDrawContainingBox(UWorld* world);
	void DEBUGDrawSurrondings(UWorld* world, FColor usedColor = FColor::Magenta);

	void AddToTree(UKDTree* box);
	bool IsPlaceEmpty(const UMinMaxBox* box);
	void GetContainingObjects(const UMinMaxBox* box, TArray<UObject*>& outArray, const UObject* ignoreElement = nullptr, const bool checkForCommonBoundaries = false);
	void GetContainingObjectsFromBottom(const UMinMaxBox* box, TArray<UObject*>& outArray, const UObject* ignoreElement);

	void UpdateAfterChildDestroyed();

	FORCEINLINE UKDTree* GetParent()
	{
		return GetParentNodeTemplate<UKDTree>();
	}

	friend bool CheckCommonBoundaries(UObject* o1, const UObject* o2);
	friend void AddToTreeElements(UObject* obj, UKDTree* box);
	friend void RemoveFromTreeElements(UObject* obj, UKDTree* box);
	friend void WatchingRegionChanged(UObject* obj);

	void NotifyRegionChanged(UMinMaxBox* box);
	void RegisterWatchingBox(UObject* actor, UMinMaxBox* box);
	void TryUnregisterWatchingBox(UObject* actor);

private:

	void addToTree(UKDTree* box, bool forceInsert = false);

	void addToTreeByCoord(UKDTree* box);

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

	FORCEINLINE bool checkElem(UKDTree*& elem)
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
