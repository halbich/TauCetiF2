#include "Blocks.h"
#include "BlockWithRelationsInfo.h"

UBlockWithRelationsInfo::UBlockWithRelationsInfo()
{
	ID = FGuid::NewGuid();
}

int32 UBlockWithRelationsInfo::RemoveRelationshipsByTargetID(const FGuid& targetID)
{
	int32 removed = 0;

	int32 index = INDEX_NONE;

	while ((index = Relationships.IndexOfByPredicate([targetID](URelationshipInfo* info) {
		return info->TargetID == targetID;
	})) != INDEX_NONE)
	{
		Relationships.RemoveAt(index);
		removed++;
	}

	return removed;
}