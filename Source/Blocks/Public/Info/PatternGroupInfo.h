

#pragma once

#include "UObject/NoExportTypes.h"
#include "Tree/MinMaxBox.h"
#include "PatternGroupInfo.generated.h"

class ABlock;

/**
 *
 */
UCLASS()
class BLOCKS_API UPatternGroupInfo : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
		TArray<ABlock*> BlocksInGroup;

	UPROPERTY(Transient)
		UMinMaxBox* WatchingBox;

	UPROPERTY(Transient)
		UMinMaxBox* TreeWatchingBox;

	UPROPERTY(Transient)
		bool IsValidForObserve;



	void RegisterBlock(ABlock* block);

	void UnregisterBlock(ABlock* block);

	friend UMinMaxBox* GetDefiningBox(ABlock* block);

	virtual void UpdateValidForObserve();

	virtual void WatchingRegionChanged();

private:
	FORCEINLINE void updateTreeWatchingBox()
	{
		auto min = WatchingBox->Min;
		auto max = WatchingBox->Max;

		min.Z = max.Z;	// move bottom toUpper border

		auto scale = (max - min);

		max.Z += FMath::Max(scale.X, scale.Y);

		TreeWatchingBox->InitBox(min, max);
	}

};
