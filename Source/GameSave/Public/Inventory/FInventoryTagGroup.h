#pragma once

#include "Inventory/FTagGroup.h"

/**
 *
 */
class GAMESAVE_API FInventoryTagGroup
{
public:
	FInventoryTagGroup();

	FString GroupName;

	bool IsGroupEnabled;

	TArray<FTagGroup> GroupList;
};
