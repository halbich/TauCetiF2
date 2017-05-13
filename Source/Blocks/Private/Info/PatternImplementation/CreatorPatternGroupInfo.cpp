#include "Blocks.h"
#include "CreatorPatternGroupInfo.h"

void UCreatorPatternGroupInfo::UpdateValidForObserve()
{
	Super::UpdateValidForObserve();

	if (!IsValidForObserve)
	{
		IsValidCreator = false;
		return;
	}

	auto scale = ((WatchingBox->Max - WatchingBox->Min) / GameDefinitions::CubeMinSize).GridSnap(1);

	IsValidForObserve = scale.X == scale.Y && scale.X >= 1 && scale.X <= 20 && scale.Z == 2;
}

void UCreatorPatternGroupInfo::WatchingRegionChanged()
{
	if (!IsValidForObserve || BlocksInGroup.Num() == 0)
	{
		IsValidCreator = false;
		return;
	}

	ComputeCreator(BlocksInGroup[0]);
}