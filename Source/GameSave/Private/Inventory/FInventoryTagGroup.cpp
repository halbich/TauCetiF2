#include "GameSave.h"
#include "FInventoryTagGroup.h"

FInventoryTagGroup::FInventoryTagGroup() : GroupName(), IsGroupEnabled(false), GroupList(), GroupType(0)
{
}

void FInventoryTagGroup::Empty() {
	GroupName.Empty();

	for (auto tg : GroupList)
		tg.Empty();

	GroupList.Empty();
}