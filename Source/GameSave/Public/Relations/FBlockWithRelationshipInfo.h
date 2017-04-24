#pragma once

#include "FRelationshipInfo.h"

/**
 *
 */
struct GAMESAVE_API FBlockWithRelationshipInfo
{
public:
	FBlockWithRelationshipInfo();

	FGuid ID;

	TArray<FRelationshipInfo> Relationships;
};
