

#include "Blocks.h"
#include "PatternGroupInfo.h"





void UPatternGroupInfo::RegisterBlock(ABlock* block)
{
	// TODO borders

	BlocksInGroup.AddUnique(block);

	auto defBox = GetDefiningBox(block);

	if (!WatchingBox || !WatchingBox->IsValidLowLevel())
	{
		WatchingBox = NewObject<UMinMaxBox>();
		WatchingBox->InitBox(defBox->Min, defBox->Max);

	}

	if (!TreeWatchingBox || !TreeWatchingBox->IsValidLowLevel())
	{
		TreeWatchingBox = NewObject<UMinMaxBox>();
		updateTreeWatchingBox();
	}
	else
	{
		FVector newMin = FVector(FMath::Min(WatchingBox->Min.X, defBox->Min.X), FMath::Min(WatchingBox->Min.Y, defBox->Min.Y), FMath::Min(WatchingBox->Min.Z, defBox->Min.Z));
		FVector newMax = FVector(FMath::Max(WatchingBox->Max.X, defBox->Max.X), FMath::Max(WatchingBox->Max.Y, defBox->Max.Y), FMath::Max(WatchingBox->Max.Z, defBox->Max.Z));

		WatchingBox->InitBox(newMin, newMax);
		updateTreeWatchingBox();
	}

	UpdateValidForObserve();
}

void UPatternGroupInfo::UnregisterBlock(ABlock* block)
{
	auto rem = BlocksInGroup.Remove(block);
	ensure(rem > 0);

	IsValidForObserve = false;
}


void UPatternGroupInfo::UpdateValidForObserve()
{
	IsValidForObserve = !(WatchingBox->Max - WatchingBox->Min).IsNearlyZero();
}

void UPatternGroupInfo::WatchingRegionChanged()
{
	if (!IsValidForObserve)
		return;

	print(TEXT("Region changed"));
}