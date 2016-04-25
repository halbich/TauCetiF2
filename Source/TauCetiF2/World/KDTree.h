

#pragma once

#include "Object.h"
#include "MinMaxBox.h"
#include "Helpers/Helpers.h"
#include "KDTree.generated.h"

class AWorldObject;
class UWorldObjectComponent;

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
		AWorldObject* ContainingObject;

	UPROPERTY()
		int32 DividingIndex;

	UPROPERTY()
		UKDTree* SingleChild;

	UPROPERTY()
		FVector DividingCoord;

	UPROPERTY()
		float DividingCoordValue;

	void DEBUGDrawContainingBox(UWorld* world);
	void DEBUGDrawSurrondings(UWorld* world, FColor usedColor = FColor::Magenta);

	void AddToTree(UKDTree* box, bool forceInsert = false);
	bool IsPlaceEmpty(const UMinMaxBox* box);
	void GetContainingObjects(const UMinMaxBox* box, TArray<AWorldObject*>& outArray, const UWorldObjectComponent* ignoreElement = nullptr);
	void GetContainingObjectsFromBottom(const UMinMaxBox* box, TArray<AWorldObject*>& outArray, const UWorldObjectComponent* ignoreElement);

	void UpdateAfterChildDestroyed();

	FORCEINLINE UKDTree* GetParent()
	{
		return GetParentNodeTemplate<UKDTree>();
	}

private:

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

};
