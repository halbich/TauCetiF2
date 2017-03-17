#pragma once

#include "FTagGroup.h"

/**
 *
 */
struct GAMESAVE_API FInventoryTagGroup
{

	FInventoryTagGroup();

	FString GroupName;

	bool IsGroupEnabled;

	uint8 GroupType;

	TArray<FTagGroup> GroupList;

	void Empty();
};
