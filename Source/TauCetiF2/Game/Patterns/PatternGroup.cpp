#include "TauCetiF2.h"
#include "PatternGroup.h"

UPatternGroup::UPatternGroup() : UObject()
{
}

void UPatternGroup::Insert(UBlockInfo* info)
{
	//TODO logic

	auto pattern = NewObject<UPatternElement>();
	pattern->BlockInfo = info;

	Patterns.Add(pattern);
}