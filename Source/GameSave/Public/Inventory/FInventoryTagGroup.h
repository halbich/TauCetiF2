#pragma once

#include "Inventory/FTagGroup.h"

/**
 *
 */
class GAMESAVE_API FInventoryTagGroup
{
public:
	FInventoryTagGroup();

	FString Name;

	bool IsEnabled;

	TArray<FTagGroup> GroupList;
};
