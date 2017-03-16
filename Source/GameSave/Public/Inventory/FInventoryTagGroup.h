#pragma once

#include "FTagGroup.h"

/**
 *
 */
struct FInventoryTagGroup
{

	FInventoryTagGroup();

	FString GroupName;

	bool IsGroupEnabled;

	uint8 GroupType;

	TArray<FTagGroup> GroupList;

	FORCEINLINE void Empty() {
		GroupName.Empty();

		for (auto tg : GroupList)
			tg.Empty();

		GroupList.Empty();
	}
};
