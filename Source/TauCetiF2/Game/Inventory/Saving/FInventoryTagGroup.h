

#pragma once

#include "Game/Inventory/Saving/FTagGroup.h"

/**
 *
 */
class TAUCETIF2_API FInventoryTagGroup
{
public:
	FInventoryTagGroup();

	FString Name;

	bool IsEnabled;

	TArray<FTagGroup> GroupList;




};
