#include "Inventory.h"
#include "TagGroup.h"

UTagGroup::UTagGroup()
{
	// TODO Localization!
	GroupName = NSLOCTEXT("TCF2LocSpace", "LC.TagGroup.NewTagGroup", "Nová skupina").ToString();
	LetVisibleAll = true;
}

UTagGroup* UTagGroup::GetEmpty()
{
	return NewObject<UTagGroup>();
}

UTagGroup* UTagGroup::GetFromTags(const TArray<FString>& tagList)
{
	auto obj = GetEmpty();

	obj->Tags.Reserve(tagList.Num());
	for (auto t : tagList) {
		auto trimmed = t.Trim();
		if (!obj->Tags.Contains<FString>(trimmed))
			obj->Tags.Add(trimmed);
	}

	return obj;
}