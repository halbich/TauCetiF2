﻿#include "GameSave.h"
#include "FTagGroup.h"

FTagGroup::FTagGroup() : GroupName(), Tags(), LetVisibleAll(true)
{
}

void FTagGroup::Empty() {
	GroupName.Empty();
	Tags.Empty();
}